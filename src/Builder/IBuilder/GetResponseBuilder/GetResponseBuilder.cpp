#include "GetResponseBuilder.hpp"

GetResponseBuilder::GetResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _removed(false),
	  _readSharedData(),
	  _response(),
	  _path(),
	  _fd(-1) {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->_locationConfig.get()->isRedirect()) {
		// return 자료형 확인한 후에 처리.
		// throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
		// 	RedirectResponseBuilder(, this->_sharedData, this->_serverConfig, this->_locationConfig));
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
	this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().end(),
												this->_readSharedData.get()->getBuffer().begin(),
												this->_readSharedData.get()->getBuffer().end());
	this->_readSharedData.get()->getBuffer().clear();
	if (this->_readSharedData.get()->getState() == RESOLVE && this->_removed == false) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_readSharedData.get()->getFd(),
															this->_readSharedData.get()->getType());
		this->_removed = true;
		return true;
	}
	return true;
}

void GetResponseBuilder::reset() {
	this->_response.reset();
	this->_sharedData.get()->getBuffer().clear();
	this->_readSharedData.get()->getBuffer().clear();
}
bool GetResponseBuilder::build() {
	return this->setBody();
}

fd_t GetResponseBuilder::findReadFile() {
	const std::string locPath = "." + this->_locationConfig.get()->getDirectives(ROOT).asString();
	const std::string serverPath = "." + this->_serverConfig.get()->getDirectives(ROOT).asString();
	const std::vector<std::string> indexVec = this->_locationConfig.get()->getDirectives(INDEX).asStrVec();
	const std::string requestTarget = this->_request.get()->second.getRequestTarget();
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
	this->_fd = this->findReadFile();
	// if (this->_fd == -1 && this->_locationConfig.get()->isAutoIndex()) // 디렉토리 리스팅 구현.
	if (this->_fd == -1)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr();
	this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(this->_fd, EVENT_READ, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
}

void GetResponseBuilder::cgiProcessing() {
	// cgi 처리
}

void GetResponseBuilder::prepare() {
	if (this->_locationConfig.get()->isCgi())
		this->cgiProcessing();
	else
		this->fileProcessing();
}