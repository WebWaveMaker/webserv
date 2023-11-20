#include "RedirectResponseBuilder.hpp"

RedirectResponseBuilder::RedirectResponseBuilder(const unsigned int statusCode, const std::string& path,
												 reactor::sharedData_t sharedData, request_t request,
												 const utils::shared_ptr<ServerConfig>& serverConfig)
	: _statusCode(statusCode),
	  _path(path),
	  _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _readSharedData(new reactor::SharedData(FD_ERROR, EVENT_READ, std::vector<char>())),
	  _response() {
	std::cout << "path: " << this->_path << std::endl;
	this->_sharedData->clear();
	this->prepare();
}

RedirectResponseBuilder::~RedirectResponseBuilder() {}

reactor::sharedData_t RedirectResponseBuilder::getProduct() {
	return this->_sharedData;
}

void RedirectResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(this->_statusCode));
}

void RedirectResponseBuilder::setHeader() {
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);

	headers["Location"] = "http://" + this->_request->second.getHeaders()["Host"] + this->_path;
	headers[CONTENT_LENGTH] = "0";
	headers.erase("Content-Type");	// redirect는 body가 없으므로 Content-Type을 지워준다
	this->_response.setHeaders(headers);
}

bool RedirectResponseBuilder::setBody() {
	return true;
}

void RedirectResponseBuilder::reset() {
	this->_response.reset();
}

bool RedirectResponseBuilder::build() {
	return this->setBody();
}

void RedirectResponseBuilder::prepare() {
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr() + CRLF;

	std::cout << raw << std::endl;
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData->setState(RESOLVE);
}