#include "RequestParser.hpp"

RequestParser::RequestParser(utils::shared_ptr<ServerConfig> serverConfig)
	: IParser(), _msgs(), _curMsg(u::nullptr_t), _serverConfig(serverConfig) {
	_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
		new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
	_curMsg = &_msgs.back();
	_curMsg->get()->second.setContentLengthReceived(0);
	_curMsg->get()->second.getBuf().clear();
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

bool RequestParser::errorRequest(void) {
	_curMsg->get()->first = HTTP_ERROR;
	getCurMsg().setErrorCode(BAD_REQUEST);
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
		this->_msgs.front()->first == LONG_BODY_DONE)
		this->_msgs.pop();
	return elem;
}

bool RequestParser::parseStartLine(std::string& buf) {
	std::cerr << "parseStartLine" << std::endl;
	std::cerr << "buf: " << buf << std::endl;
	std::cerr << "curMsg buf: " << _curMsg->get()->second.getBuf() << std::endl;
	std::string str = this->findAndSubstr(buf, CRLF);
	std::cerr << "after parseStartLine" << std::endl;
	std::cerr << "buf: " << buf << std::endl;
	std::cerr << "curMsg buf: " << _curMsg->get()->second.getBuf() << std::endl;
	if (str.size() == 0)
		return false;
	std::stringstream ss(str);
	std::vector<std::string> startLine(3);

	ss >> startLine[0] >> startLine[1] >> startLine[2];
	if (startLine[0].empty() || startLine[1].empty() || startLine[2].empty()) {
		_curMsg->get()->first = HTTP_ERROR;
		getCurMsg().setErrorCode(BAD_REQUEST);
		return errorRequest();
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
		headers[TRANSFER_ENCODING] != "chunked") {
		_curMsg->get()->first = HTTP_ERROR;
		getCurMsg().setErrorCode(LENGTH_REQUIRED);
		return false;
	}
	if (headers.count(CONTENT_LENGTH) == 0)
		headers[CONTENT_LENGTH] = "0";
	getCurMsg().setContentLength(utils::stoui(headers[CONTENT_LENGTH]));
	getCurMsg().setHeaders(headers);
	if (headers[TRANSFER_ENCODING] == "chunked")
		_curMsg->get()->first = CHUNKED;
	else if (getCurMsg().getContentLength() > BUFFER_SIZE)
		_curMsg->get()->first = LONG_FIRST;
	else
		_curMsg->get()->first = BODY;

	return true;
}

bool RequestParser::parseBody(std::string& buf) {
	_curMsg->get()->first = DONE;
	const std::map<std::string, std::string>& headers = getCurMsg().getHeaders();
	if (this->checkContentLengthZero(headers))
		return true;
	const unsigned int contentLength = getCurMsg().getContentLength();
	if (contentLength > _serverConfig->getDirectives(CLIENT_MAX_BODY_SIZE).asUint()) {
		_curMsg->get()->first = HTTP_ERROR;
		getCurMsg().setErrorCode(PAYLOAD_TOO_LARGE);
		return false;
	};
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

bool RequestParser::parseChunked(std::string& buf) {
	unsigned int contentLength = utils::stoui(this->findAndSubstr(buf, CRLF));
	if (contentLength == 0) {
		_curMsg->get()->first = DONE;
		buf.clear();
		return true;
	}
	const std::string str = buf.substr(0, contentLength - 1);
	buf.erase(0, contentLength - 1);
	_curMsg->get()->second.setChunkedBody(str);
	return true;
}

bool RequestParser::parseLongBody(std::string& buf) {
	getCurMsg().setBody(buf);
	getCurMsg().setContentLengthReceived(getCurMsg().getContentLengthReceived() + buf.size());
	const unsigned int contentLength = getCurMsg().getContentLength();
	const unsigned int contentLengthReceived = getCurMsg().getContentLengthReceived();

	if (contentLength == contentLengthReceived) {
		_curMsg->get()->first = LONG_BODY_DONE;
	}
	if (contentLength > _serverConfig->getDirectives(CLIENT_MAX_BODY_SIZE).asUint()) {
		_curMsg->get()->first = LONG_BODY_ERROR;
		getCurMsg().setErrorCode(PAYLOAD_TOO_LARGE);
		return false;
	};
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

	std::cerr << "request in state: " << this->_curMsg->get()->first << std::endl;
	std::cerr << "request content" << this->_curMsg->get()->second.getBuf() << std::endl;
	std::cerr << "current buf" << this->_curMsg->get()->second.getBuf() << std::endl;
	std::cerr << "request in content: " << content << std::endl;

	// std::cerr << "previous contentLengthReceived: " << getCurMsg().getContentLengthReceived() << std::endl;
	while (content.empty() == false) {
		this->branchParser(this->_msgs.back()->first, content);
		if (this->_msgs.back()->first == DONE && content.empty() == false) {
			_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
				new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
			_curMsg = &_msgs.back();
		}
	}
	if (content.empty() && getCurMsg().getBuf().size())
		this->branchParser(_curMsg->get()->first, content);
	if (this->_curMsg->get()->first == BODY &&
		(getCurMsg().getHeaders().count(CONTENT_LENGTH) == 0 || getCurMsg().getHeaders().at(CONTENT_LENGTH) == "0"))
		this->_curMsg->get()->first = DONE;
	std::cerr << "request out state: " << this->_curMsg->get()->first << std::endl;
	return this->pop();
}
