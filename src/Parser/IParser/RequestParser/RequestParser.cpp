#include "RequestParser.hpp"

RequestParser::RequestParser(utils::shared_ptr<ServerConfig> serverConfig)
	: IParser(), _msgs(), _curMsg(u::nullptr_t), _serverConfig(serverConfig) {
	_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
		new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
	_curMsg = &_msgs.back();
	_curMsg->get()->second.reset();
}

RequestParser::~RequestParser() {}

bool RequestParser::checkContentLengthZero(const std::map<std::string, std::string>& headers) {
	return headers.count(CONTENT_LENGTH) == 0 || headers.at(CONTENT_LENGTH) == "0";
}

HttpMessage& RequestParser::getCurMsg(void) {
	return this->_curMsg->get()->second;
}

enum AsyncState RequestParser::getState(void) const {
	return this->_msgs.empty() ? RESOLVE : PENDING;
}

bool RequestParser::setErrorRequest(const enum HttpMessageState state, const enum HTTP_STATUS code) {
	_curMsg->get()->first = state;
	getCurMsg().setErrorCode(code);
	return false;
}

std::string RequestParser::findAndSubstr(std::string& buf, std::string delim) {

	std::string& tmpBuf = getCurMsg().getBuf();
	if (tmpBuf.size()) {
		buf = tmpBuf + buf;
		tmpBuf.clear();
	}
	std::string::size_type size = buf.find(delim);
	if (size == std::string::npos) {
		tmpBuf += buf;
		buf.clear();
		return "";
	}
	std::string str = buf.substr(0, size);
	if (str == CRLF) {
		tmpBuf += buf;
		buf.clear();
		return "";
	}
	buf = buf.substr(size + delim.size());
	return str;
}

request_t RequestParser::get(void) {
	return this->pop();
}

request_t RequestParser::pop(void) {
	request_t elem;
	if (this->_msgs.empty() || this->_msgs.front()->first == START_LINE || this->_msgs.front()->first == HEADER ||
		this->_msgs.front()->first == BODY)
		return elem;
	elem = this->_msgs.front();
	if (this->_msgs.front()->first == DONE || this->_msgs.front()->first == HTTP_ERROR ||
		this->_msgs.front()->first == LONG_BODY_DONE || this->_msgs.front()->first == CHUNKED_DONE ||
		this->_msgs.front()->first == LONG_BODY_ERROR || this->_msgs.front()->first == CHUNKED_ERROR)
		this->_msgs.pop();
	return elem;
}

bool RequestParser::parseStartLine(std::string& buf) {
	std::string str = this->findAndSubstr(buf, CRLF);
	if (str.size() == 0)
		return false;
	std::stringstream ss(str);
	std::vector<std::string> startLine(3);

	ss >> startLine[0] >> startLine[1] >> startLine[2];
	if (startLine[0].empty() || startLine[1].empty() || startLine[2].empty()) {
		return setErrorRequest(HTTP_ERROR, BAD_REQUEST);
	}
	getCurMsg().setStartLine(startLine);
	_curMsg->get()->first = HEADER;
	return true;
}

bool RequestParser::parseHeader(std::string& buf) {
	std::string str = this->findAndSubstr(buf, RNRN);
	if (str.size() == 0)
		return false;
	str += CRLF;
	std::vector<std::string> headerFields = utils::split(str, CRLF);
	std::map<std::string, std::string> headers;

	for (std::vector<std::string>::iterator it = headerFields.begin(); it != headerFields.end(); ++it) {
		const std::string::size_type colonPos = (*it).find(':');
		if (colonPos == std::string::npos)
			continue;
		const std::string key = (*it).substr(0, colonPos);
		const std::string val = utils::trim((*it).substr(colonPos + 1));

		headers[key] = val;
	}
	if ((getCurMsg().getMethod() == POST || getCurMsg().getMethod() == PUT) && this->checkContentLengthZero(headers) &&
		headers[TRANSFER_ENCODING] != "chunked")
		return setErrorRequest(HTTP_ERROR, LENGTH_REQUIRED);
	if (headers[CONTENT_TYPE] == MULTIPART_FORM_DATA)
		return setErrorRequest(HTTP_ERROR, UNSUPPORTED_MEDIA_TYPE);
	if (headers.count(CONTENT_LENGTH) == 0)
		headers[CONTENT_LENGTH] = "0";
	getCurMsg().setContentLength(utils::stoui(headers[CONTENT_LENGTH]));
	getCurMsg().setHeaders(headers);
	if (headers[TRANSFER_ENCODING] == "chunked")
		_curMsg->get()->first = CHUNKED_FIRST;
	else if (getCurMsg().getContentLength() > BUFFER_SIZE)
		_curMsg->get()->first = LONG_FIRST;
	else
		_curMsg->get()->first = BODY;
	this->_curMsg->get()->second.setContentLengthReceived(0);
	this->_curMsg->get()->second.setTotalChunkedLength(0);

	return true;
}

bool RequestParser::parseBody(std::string& buf) {
	_curMsg->get()->first = DONE;
	const std::map<std::string, std::string>& headers = getCurMsg().getHeaders();
	if (this->checkContentLengthZero(headers))
		return true;
	const unsigned int contentLength = getCurMsg().getContentLength();
	if (contentLength > _serverConfig->getDirectives(CLIENT_MAX_BODY_SIZE).asUint())
		return setErrorRequest(HTTP_ERROR, PAYLOAD_TOO_LARGE);
	try {
		const std::string str = buf.substr(0, contentLength);
		buf.erase(0, contentLength);
		getCurMsg().setBody(str);
	} catch (const std::out_of_range& ex) {
		ErrorLogger::parseError(__FILE__, __LINE__, __func__, "content-length too large compared of body");
		_curMsg->get()->first = HTTP_ERROR;
		getCurMsg().setErrorCode(BAD_REQUEST);
		return false;
	}
	return true;
}

inline void RequestParser::saveBufAndClear(std::string& buf) {
	getCurMsg().setBuf(buf);
	buf.clear();
}

inline std::string RequestParser::cutBuf(std::string& buf, const std::string::size_type size) {
	std::string rv = buf.substr(0, size);
	buf.erase(0, size);
	return rv;
}

bool RequestParser::parseChunked(std::string& buf) {
	HttpMessage& curMsg = getCurMsg();
	buf = curMsg.getBuf() + buf;
	curMsg.getBuf().clear();
	std::string::size_type crlf_pos = buf.find(CRLF);

	if (crlf_pos == std::string::npos) {
		saveBufAndClear(buf);
		return false;
	}
	unsigned int chunkedLength = 0;
	try {
		chunkedLength = utils::toHexNum<unsigned int>(buf.substr(0, crlf_pos));
	} catch (const std::invalid_argument& ex) {
		ErrorLogger::parseError(__FILE__, __LINE__, __func__, "chunked length is not hex");
		return setErrorRequest(HTTP_ERROR, BAD_REQUEST);
	}

	if (buf.size() < crlf_pos + CRLF_LEN + chunkedLength + CRLF_LEN) {
		saveBufAndClear(buf);
		return false;
	}

	curMsg.setContentLengthReceived(curMsg.getContentLengthReceived() + chunkedLength);
	curMsg.setTotalChunkedLength(curMsg.getTotalChunkedLength() + chunkedLength);
	if (curMsg.getTotalChunkedLength() >= _serverConfig->getDirectives(CLIENT_MAX_BODY_SIZE).asUint())
		return setErrorRequest(HTTP_ERROR, PAYLOAD_TOO_LARGE);
	cutBuf(buf, crlf_pos + CRLF_LEN);
	curMsg.setChunkedBody(cutBuf(buf, chunkedLength));
	if (cutBuf(buf, CRLF_LEN) != CRLF) {
		ErrorLogger::parseError(__FILE__, __LINE__, __func__, "chunked body is not CRLF");
		return setErrorRequest(HTTP_ERROR, BAD_REQUEST);
	}
	if (chunkedLength == 0 && curMsg.getTotalChunkedLength() < BUFFER_SIZE) {
		_curMsg->get()->first = DONE;
		return true;
	} else if (chunkedLength == 0)
		_curMsg->get()->first = CHUNKED_DONE;
	else
		_curMsg->get()->first = CHUNKED;
	if (_curMsg->get()->first == CHUNKED && curMsg.getIsRegistered() == false && chunkedLength != 0)
		_curMsg->get()->first = CHUNKED_FIRST;
	return true;
}

bool RequestParser::parseLongBody(std::string& buf) {
	getCurMsg().setBody(buf);
	getCurMsg().setContentLengthReceived(getCurMsg().getContentLengthReceived() + buf.size());
	const unsigned int contentLength = getCurMsg().getContentLength();
	const unsigned int contentLengthReceived = getCurMsg().getContentLengthReceived();

	if (contentLength == contentLengthReceived)
		_curMsg->get()->first = LONG_BODY_DONE;
	if (contentLength > _serverConfig->getDirectives(CLIENT_MAX_BODY_SIZE).asUint())
		return setErrorRequest(LONG_BODY_ERROR, PAYLOAD_TOO_LARGE);
	buf.clear();
	return true;
}

void RequestParser::branchParser(const enum HttpMessageState state, std::string& buf) {
	switch (state) {
		case START_LINE:
			this->parseStartLine(buf);
			break;
		case HEADER:
			this->parseHeader(buf);
			break;
		case BODY:
			this->parseBody(buf);
			break;
		case CHUNKED_FIRST:
			this->parseChunked(buf);
			break;
		case CHUNKED:
			this->parseChunked(buf);
			break;
		case LONG_FIRST:
			this->parseLongBody(buf);
			break;
		case LONG_BODY:
			this->parseLongBody(buf);
			break;
		case HTTP_ERROR:
			buf.clear();
			break;
		case LONG_BODY_ERROR:
			buf.clear();
			break;
		case CHUNKED_ERROR:
			buf.clear();
			break;
		default:
			break;
	}
}

request_t RequestParser::parse(std::string& content) {
	if (content.size() == 0)
		return this->pop();
	if (this->_msgs.empty()) {
		_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
			new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
		_curMsg = &_msgs.back();
	}
	while (content.empty() == false) {
		this->branchParser(this->_msgs.back()->first, content);
		if (this->_msgs.back()->first == DONE && content.empty() == false) {
			_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
				new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
			_curMsg = &_msgs.back();
		}
	}
	if (this->_curMsg->get()->first == BODY && checkContentLengthZero(getCurMsg().getHeaders()))
		this->_curMsg->get()->first = DONE;
	return this->pop();
}
