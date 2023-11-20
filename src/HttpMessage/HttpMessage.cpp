#include "HttpMessage.hpp"

HttpMessage::HttpMessage()
	: _startLine(3), _headers(), _body(), _errorCode(0), _contentLength(0), _contentLengthReceived(0) {}

HttpMessage::HttpMessage(const HttpMessage& obj) {
	*this = obj;
}

HttpMessage::~HttpMessage() {}

HttpMessage& HttpMessage::operator=(const HttpMessage& obj) {
	if (this != &obj) {
		this->_startLine = obj._startLine;
		this->_headers = obj._headers;
		this->_body = obj._body;
	}
	return *this;
}

std::string HttpMessage::combineHeaders(void) {
	std::string httpRequest;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
		httpRequest += it->first + ": " + it->second + "\r\n";
	}
	return httpRequest;
}

std::string HttpMessage::getRawStr(void) {
	const std::string startLine = utils::join(_startLine, " ") + "\r\n";
	const std::string headers = this->combineHeaders() + "\r\n";

	return startLine + headers;
}

void HttpMessage::setErrorCode(const int errorCode) {
	this->_errorCode = errorCode;
}

int HttpMessage::getErrorCode(void) const {
	return this->_errorCode;
}

std::map<std::string, std::string>& HttpMessage::getHeaders(void) {
	return this->_headers;
}

void HttpMessage::setStartLine(const std::vector<std::string> startLine) {
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

std::string HttpMessage::getRequestTarget(void) const {
	return this->_startLine[1];
}

std::string HttpMessage::getTargetFile(void) const {
	std::string::size_type pos = this->_startLine[1].find_last_of('/', this->_startLine[1].length() - 1);

	return this->_startLine[1].substr(pos + 1);
}

std::string& HttpMessage::getBody(void) {
	return this->_body;
}

void HttpMessage::setHeaders(const std::map<std::string, std::string>& headers) {
	this->_headers = headers;
}

void HttpMessage::setBody(const std::string& body) {
	this->_body = body;
}

void HttpMessage::setChunkedBody(const std::string& body) {
	this->_body += body;
}

void HttpMessage::reset() {
	this->_startLine[0].clear();
	this->_startLine[1].clear();
	this->_startLine[2].clear();
	this->_headers.clear();
	this->_body.clear();
}

unsigned int HttpMessage::getContentLength(void) const {
	return this->_contentLength;
}

void HttpMessage::setContentLength(const unsigned int contentLength) {
	this->_contentLength = contentLength;
}

unsigned int HttpMessage::getContentLengthReceived(void) const {
	return this->_contentLengthReceived;
}

void HttpMessage::setContentLengthReceived(const unsigned int contentLengthReceived) {
	this->_contentLengthReceived = contentLengthReceived;
}