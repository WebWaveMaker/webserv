#include "GetResponseBuilder.hpp"

GetResponseBuilder::GetResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& config)
	: _sharedData(sharedData), _request(request), _serverConfig(config) {}

GetResponseBuilder::~GetResponseBuilder() {}

reactor::sharedData_t GetResponseBuilder::getProduct() {
	return this->_sharedData;
}

void GetResponseBuilder::setStartLine() {
	std::string startLine[3];
	startLine[0] = "HTTP/1.1";
	startLine[1] = "200";
	startLine[2] = "OK";
	this->_response.setStartLine(startLine);
}

void GetResponseBuilder::setHeader() {
	const std::string path = "/Users/jgo/Programming/devjgo/webserv/var/www/index.html";
	struct stat fileInfo;

	if (stat(path.c_str(), &fileInfo) < 0) {
		throw std::runtime_error("stat error");	 // throw ErrorResponseBuilder(404) internal server error later
	}

	std::map<std::string, std::string> headers;
	headers["Server"] = this->_serverConfig.get()->getDirectives(SERVER_NAME).asString();
	headers["Date"] = utils::getCurTime(logTimeFormat::UTCtimeFormat);
	headers["Content-Type"] = this->_serverConfig.get()->getDirectives(DEFAULT_TYPE).asString();
	headers["Content-Length"] = utils::lltos(fileInfo.st_size);
	this->_response.setHeaders(headers);
}

bool GetResponseBuilder::setBody() {
	// this->_sharedData.get()->getBuffer().insert();
	return true;
}

void GetResponseBuilder::reset() {
	this->_response.reset();
	this->_sharedData.get()->getBuffer().clear();
}
bool GetResponseBuilder::build() {
	return this->setBody();
}
void GetResponseBuilder::prepare() {
	this->setStartLine();
	this->setHeader();
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
	
}