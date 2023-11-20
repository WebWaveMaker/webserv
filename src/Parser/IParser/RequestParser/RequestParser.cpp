#include "RequestParser.hpp"

RequestParser::RequestParser(utils::shared_ptr<ServerConfig> serverConfig)
	: IParser(), _msgs(), _curMsg(u::nullptr_t), _buf(), _serverConfig(serverConfig) {
	_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
		new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
	_curMsg = &_msgs.back();
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

std::string RequestParser::findAndSubstr(std::string& buf, std::string delim) {
	std::string::size_type size = buf.find(delim);
	if (size == std::string::npos) {
		this->_buf += buf;
		buf.clear();
		return "";
	}
	if (this->_buf.size()) {
		buf = this->_buf + buf;
		size += this->_buf.size();
		this->_buf.clear();
	}
	std::string str = buf.substr(0, size);
	buf = buf.substr(size + delim.size());
	return str;
}

request_t RequestParser::get(void) {
	return this->pop();
}

request_t RequestParser::pop(void) {
	request_t elem;
	if (this->_msgs.empty())
		return elem;
	if (this->_msgs.front()->first == DONE || this->_msgs.front()->first == HTTP_ERROR) {
		elem = this->_msgs.front();
		this->_msgs.pop();
	}
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
		_curMsg->get()->first = HTTP_ERROR;
		getCurMsg().setErrorCode(BAD_REQUEST);
		return false;
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
	if ((getCurMsg().getMethod() == POST || getCurMsg().getMethod() == PUT) && this->checkContentLengthZero(headers)) {
		_curMsg->get()->first = HTTP_ERROR;
		getCurMsg().setErrorCode(LENGTH_REQUIRED);
		return false;
	}
	if (headers.count(CONTENT_LENGTH) == 1)
		getCurMsg().setContentLength(utils::stoui(headers[CONTENT_LENGTH]));
	else
		headers[CONTENT_LENGTH] = "0";
	getCurMsg().setHeaders(headers);
	_curMsg->get()->first = BODY;
	return true;
}

bool RequestParser::parserBody(std::string& buf) {
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

request_t RequestParser::parse(const std::string& content) {
	if (content.size() == 0)
		return this->pop();
	if (this->_msgs.empty()) {
		_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
			new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
		_curMsg = &_msgs.back();
	}
	std::string buf(content);

	while (buf.empty() == false) {
		switch (this->_msgs.back()->first) {
			case START_LINE:
				this->parseStartLine(buf);
				break;
			case HEADER:
				this->parseHeader(buf);
				break;
			case BODY:
				this->parserBody(buf);
				break;
			case HTTP_ERROR:
				buf.clear();
			default:
				break;
		}
		if (this->_msgs.back()->first == DONE && buf.empty() == false) {
			_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
				new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
			_curMsg = &_msgs.back();
		}
	}
	if (this->_curMsg->get()->first == BODY &&
		(getCurMsg().getHeaders().count(CONTENT_LENGTH) == 0 || getCurMsg().getHeaders().at(CONTENT_LENGTH) == "0"))
		this->_curMsg->get()->first = DONE;
	return this->pop();
}
