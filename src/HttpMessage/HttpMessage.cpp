#include "HttpMessage.hpp"

HttpMessage::HttpMessage() : _startLine(), _headers(), _body() {}
HttpMessage::HttpMessage(const HttpMessage& obj) {
	*this = obj;
}

HttpMessage::~HttpMessage() {}

HttpMessage& HttpMessage::operator=(const HttpMessage& obj) {
	if (this != &obj) {
		this->_startLine[0] = obj._startLine[0];
		this->_startLine[1] = obj._startLine[1];
		this->_startLine[2] = obj._startLine[2];
		this->_headers = obj._headers;
		this->_body = obj._body;
	}
	return *this;
}

std::string HttpMessage::getRawStr(void) const {
	return _startLine[0] + " " + _startLine[1] + " " + _startLine[2];
}

std::map<std::string, std::string>& HttpMessage::getHeaders(void) {
	return this->_headers;
}

void HttpMessage::setStartLine(const std::string startLine[3]) {
	this->_startLine[0] = startLine[0];	 // method | Http-version
	this->_startLine[1] = startLine[1];	 // request-target | status code
	this->_startLine[2] = startLine[2];	 // HTTP-version | reason-phrase
}

std::string HttpMessage::getMethod(void) const {
	return this->_startLine[0];
}

void HttpMessage::setHeaders(const std::map<std::string, std::string>& headers) {
	this->_headers = headers;
}

void HttpMessage::setBody(const std::string& body) {
	this->_body = body;
}

// void HttpMessage::setBuf(const std::string& buf) {
// 	this->_buf = buf;
// }

// void HttpMessage::setRemain(const std::string& remain) {
// 	this->_remain = remain;
// }