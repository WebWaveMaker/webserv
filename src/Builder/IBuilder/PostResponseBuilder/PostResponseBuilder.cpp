#include "PostResponseBuilder.hpp"

PostResponseBuilder::PostResponseBuilder(reactor::sharedData_t sharedData, request_t request,
										 const utils::shared_ptr<ServerConfig>& serverConfig,
										 const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _response(),
	  _readSharedData() {
	if (request->second.getBody().empty())
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->prepare();
};

PostResponseBuilder::~PostResponseBuilder() {}

reactor::sharedData_t PostResponseBuilder::getProduct() {
	return this->_sharedData;
}

void PostResponseBuilder::divideEntryPoint() {
	std::string targetTarget = this->_request->second.getRequestTarget();

	if (targetTarget == "/signup")

	if (targetTarget == "/login")

	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
		new ErrorResponseBuilder(BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
}

void PostResponseBuilder::setStartLine() {}
void PostResponseBuilder::setHeader() {}
bool PostResponseBuilder::setBody() {
	return true;
}
void PostResponseBuilder::reset() {}
bool PostResponseBuilder::build() {
	return this->setBody();
}
void PostResponseBuilder::prepare() {
	this->divideEntryPoint();
}