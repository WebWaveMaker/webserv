#include "GetResponseBuilder.hpp"

GetResponseBuilder::GetResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _removed(false),
	  _readSharedData() {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(404, this->_sharedData, this->_serverConfig, this->_locationConfig));
	// if (this->_locationConfig.get()->isRedirect())
	//	throw RedirectResponseBuilder(); // throw utils::shared_ptr<IBuilder<sharedData_t> >(ErrorResponseBuilder(301) 301 moved permanently
	this->prepare();
}

GetResponseBuilder::~GetResponseBuilder() {
	close(this->_fd);
}

reactor::sharedData_t GetResponseBuilder::getProduct() {
	return this->_readSharedData;
}

void GetResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(200));
}

void GetResponseBuilder::setHeader() {
	struct stat fileInfo;

	// cgi 처리는 다르게 해야함.
	if (stat(this->_path.c_str(), &fileInfo) == -1) {
		throw std::runtime_error(
			"stat error");	// throw utils::shared_ptr<IBuilder<sharedData_t> >(ErrorResponseBuilder(500) internal server error later
	}

	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig);
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
	if (this->_readSharedData->getState() == TERMINATE) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_readSharedData.get()->getFd(),
															this->_readSharedData.get()->getType());
		this->_sharedData->setState(TERMINATE);
		throw false;
	}
	return this->setBody();
}

fd_t GetResponseBuilder::findReadFile() {
	const std::string locPath = "." + this->_locationConfig.get()->getDirectives(ROOT).asString();
	const std::string serverPath = "." + this->_serverConfig.get()->getDirectives(ROOT).asString();
	const std::vector<std::string> indexVec = this->_locationConfig.get()->getDirectives(INDEX).asStrVec();

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
	if (this->_fd == -1)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(404, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr();
	this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(this->_fd, EVENT_READ, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
}

// 해당 함수에서 cgi실행의 유효성을 검사해서 넘어간다.
void GetResponseBuilder::cgiProcessing() {}

void GetResponseBuilder::prepare() {
	if (this->_locationConfig.get()->isCgi())
		this->cgiProcessing();
	else
		this->fileProcessing();
}
