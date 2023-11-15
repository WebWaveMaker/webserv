#include "RedirectResponseBuilder.hpp"

RedirectResponseBuilder::RedirectResponseBuilder(const unsigned int statusCode, const std::string& path,
												 reactor::sharedData_t sharedData,
												 const utils::shared_ptr<ServerConfig>& serverConfig,
												 const utils::shared_ptr<LocationConfig>& locationConfig)
	: statusCode(statusCode),
	  path(path),
	  _sharedData(sharedData),
	  _request(),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _readSharedData() {
	std::cout << path << std::endl;
}
RedirectResponseBuilder::~RedirectResponseBuilder() {}

reactor::sharedData_t RedirectResponseBuilder::getProduct() {
	return this->_readSharedData;
}
void RedirectResponseBuilder::setStartLine() {}
void RedirectResponseBuilder::setHeader() {}
bool RedirectResponseBuilder::setBody() {
	return true;
}
void RedirectResponseBuilder::reset() {}
bool RedirectResponseBuilder::build() {
	return this->setBody();
}
void RedirectResponseBuilder::prepare() {}