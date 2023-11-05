#include "GetResponseBuilder.hpp"

GetResponseBuilder::GetResponseBuilder(utils::shared_ptr<ServerConfig> config, msg_t _response, std::string filedata)
	: _config(config), _filedata(filedata), _response(_response) {
	this->setStartLine();
	this->setHeader();
	this->setBody();
}

GetResponseBuilder::~GetResponseBuilder() {}

msg_t GetResponseBuilder::getProduct() {
	return this->_response;
}

void GetResponseBuilder::setStartLine() {
	std::string startLine[3];
	startLine[0] = "HTTP/1.1";
	startLine[1] = "200";
	startLine[2] = "OK";
	this->_response.get()->setStartLine(startLine);
}

void GetResponseBuilder::setHeader() {
	std::map<std::string, std::string> headers;
	headers["Server"] = this->_config.get()->getDirectives(SERVER_NAME).asString();
	headers["Date"] = utils::getCurTime(logTimeFormat::UTCtimeFormat);
	headers["Content-Type"] = this->_config.get()->getDirectives(DEFAULT_TYPE).asString();
	headers["Content-Length"] = std::to_string(this->_filedata.length());
	this->_response.get()->setHeaders(headers);
}

void GetResponseBuilder::setBody() {
	this->_response.get()->setBody(this->_filedata);
}
