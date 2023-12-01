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
	  _response(),
	  _sessionId("") {
	this->_sharedData->clear();
	this->prepare();
}

RedirectResponseBuilder::RedirectResponseBuilder(const unsigned int statusCode, const std::string& path,
												 reactor::sharedData_t sharedData, request_t request,
												 const utils::shared_ptr<ServerConfig>& serverConfig,
												 const sessionId_t sessionId)
	: _statusCode(statusCode),
	  _path(path),
	  _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _readSharedData(new reactor::SharedData(FD_ERROR, EVENT_READ, std::vector<char>())),
	  _response(),
	  _sessionId(sessionId) {
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
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig);

	if (this->_path.compare(0, 5, "/http") == 0)
		headers["Location"] = this->_path.substr(1);
	else
		headers["Location"] = "http://" + this->_request->second.getHeaders()["Host"] + this->_path;
	headers[CONTENT_LENGTH] = "0";
	if (this->_sessionId != "")
		headers[SET_COOKIE] = "sessionId=" + this->_sessionId + "; httponly; Max-Age=60;";
	this->_response.setHeaders(headers);
}

bool RedirectResponseBuilder::setBody() {
	if (this->_readSharedData->getBuffer().empty())
		return false;
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
	const std::string raw = this->_response.getRawStr();

	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData->setState(RESOLVE);
}

utils::shared_ptr<IBuilder<reactor::sharedData_t> > RedirectResponseBuilder::createRedirectResponseBuilder(
	const unsigned int statusCode, const std::string& path, reactor::sharedData_t sharedData, request_t request,
	const utils::shared_ptr<ServerConfig>& serverConfig) {
	return utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
		new RedirectResponseBuilder(statusCode, path, sharedData, request, serverConfig));
}
