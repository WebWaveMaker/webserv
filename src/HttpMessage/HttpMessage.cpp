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

enum HttpMethods HttpMessage::getMethod(void) const {
	const std::string method = this->_startLine[0];

	if (method == "GET")
		return GET;
	if (method == "POST")
		return POST;
	if (method == "DELETE")
		return DELETE;
	if (method == "PUT")
		return PUT;
	return UNKNOWN;
}

void HttpMessage::setHeaders(const std::map<std::string, std::string>& headers) {
	this->_headers = headers;
}

void HttpMessage::setBody(const std::string& body) {
	this->_body = body;
}

void HttpMessage::reset() {
	this->_startLine[0].clear();
	this->_startLine[1].clear();
	this->_startLine[2].clear();
	this->_headers.clear();
	this->_body.clear();
}
