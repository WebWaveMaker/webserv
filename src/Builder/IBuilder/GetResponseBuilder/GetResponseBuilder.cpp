#include "GetResponseBuilder.hpp"

GetResponseBuilder::GetResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _removed(false),
	  _path(),
	  _fd(-1),
	  _readSharedData(),
	  _response() {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->_locationConfig->isRedirect()) {
		std::vector<std::string> rv = this->_locationConfig->getDirectives(RETURN).asStrVec();

		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			utils::stoui(rv[0]), rv[1], this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	this->prepare();
}

GetResponseBuilder::~GetResponseBuilder() {
	close(this->_fd);
}

reactor::sharedData_t GetResponseBuilder::getProduct() {
	return this->_readSharedData;
}

void GetResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(OK));
}

void GetResponseBuilder::setHeader() {
	struct stat fileInfo;

	// cgi 처리는 다르게 해야함.
	if (stat(this->_path.c_str(), &fileInfo) == -1) {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	if (fileInfo.st_size == 0) {
		close(this->_fd);
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);
	headers["Content-Length"] = utils::lltos(fileInfo.st_size);
	this->_response.setHeaders(headers);
}

bool GetResponseBuilder::setBody() {
	if (this->_readSharedData.get() == u::nullptr_t)
		return false;
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().end(),
										  this->_readSharedData->getBuffer().begin(),
										  this->_readSharedData->getBuffer().end());
	this->_readSharedData->getBuffer().clear();
	if (this->_readSharedData->getState() == RESOLVE && this->_removed == false) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_readSharedData->getFd(),
															this->_readSharedData->getType());
		this->_removed = true;
		return true;
	}
	return true;
}

void GetResponseBuilder::reset() {
	this->_response.reset();
	this->_sharedData->getBuffer().clear();
	this->_readSharedData->getBuffer().clear();
}
bool GetResponseBuilder::build() {
	return this->setBody();
}

fd_t GetResponseBuilder::findReadFile() {
	const std::string locPath = "." + this->_locationConfig->getDirectives(ROOT).asString();
	const std::string serverPath = "." + this->_serverConfig->getDirectives(ROOT).asString();
	const std::vector<std::string> indexVec = this->_locationConfig->getDirectives(INDEX).asStrVec();
	const std::string requestTarget = this->_request->second.getRequestTarget();
	const std::string uri = requestTarget.substr(requestTarget.find_last_of('/') + 1);

	if (uri != "") {
		this->_path = locPath + uri;
		if (access(this->_path.c_str(), R_OK) == 0)
			return utils::makeFd(this->_path.c_str(), "r");
		this->_path = serverPath + uri;
		if (access(this->_path.c_str(), R_OK) == 0)
			return utils::makeFd(this->_path.c_str(), "r");
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}

	for (std::vector<std::string>::const_iterator cit = indexVec.begin(); cit != indexVec.end(); ++cit) {
		this->_path = locPath + *cit;
		if (access(this->_path.c_str(), R_OK) == 0)
			return utils::makeFd(this->_path.c_str(), "r");
	}

	for (std::vector<std::string>::const_iterator cit = indexVec.begin(); cit != indexVec.end(); ++cit) {
		this->_path = serverPath + *cit;
		if (access(this->_path.c_str(), R_OK) == 0)
			return utils::makeFd(this->_path.c_str(), "r");
	}
	return -1;
}

void GetResponseBuilder::fileProcessing() {
	if (this->checkFileMode("." + this->_locationConfig->getDirectives(ROOT).asString()) == MODE_DIRECTORY)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new RedirectResponseBuilder(MOVED_PERMANENTLY, this->_request->second.getRequestTarget() + "/",
										this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->_fd = this->findReadFile();
	if (this->_fd == -1)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr();
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(this->_fd, EVENT_READ, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
}

void GetResponseBuilder::directoryProcessing() {}

void GetResponseBuilder::cgiProcessing() {}

void GetResponseBuilder::regularProcessing() {
	if (this->checkOurFileMode(this->_request->second.getRequestTarget()) == MODE_FILE)
		this->fileProcessing();
	else
		this->directoryProcessing();
}

void GetResponseBuilder::prepare() {
	if (this->_locationConfig->isCgi())
		this->cgiProcessing();
	else
		this->regularProcessing();
}