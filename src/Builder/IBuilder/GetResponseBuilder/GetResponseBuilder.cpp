#include "GetResponseBuilder.hpp"

GetResponseBuilder::GetResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& config)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(config),
	  _locationConfig(config.get()->getLocationConfig(request.get()->second.getRequestTarget())),
	  _readSharedData() {
	if (this->_request.get()->first == ERROR)
		throw std::runtime_error("request is error");  // throw ErrorResponseBuilder(400)  400 bad request
	if (_locationConfig.get() == u::nullptr_t)
		throw std::runtime_error("location config is null");  // throw ErrorResponseBuilder(404)  404 not found
	// if (this->_locationConfig.get()->isRedirect())
	//	throw RedirectResponseBuilder(); // throw ErrorResponseBuilder(301) 301 moved permanently
	this->prepare();
}

GetResponseBuilder::~GetResponseBuilder() {
	close(this->_fd);
}

reactor::sharedData_t GetResponseBuilder::getProduct() {
	return this->_sharedData;
}

void GetResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(200));
}

void GetResponseBuilder::setHeader() {
	struct stat fileInfo;

	// cgi 처리는 다르게 해야함.
	if (stat(this->path.c_str(), &fileInfo) < 0) {
		throw std::runtime_error("stat error");	 // throw ErrorResponseBuilder(500) internal server error later
	}

	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig);
	headers["Content-Length"] = utils::lltos(fileInfo.st_size);
	this->_response.setHeaders(headers);
}

bool GetResponseBuilder::setBody() {
	if (this->_readSharedData.get()->getBuffer().empty())
		return false;
	this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().end(),
												this->_readSharedData.get()->getBuffer().begin(),
												this->_readSharedData.get()->getBuffer().end());
	if (this->_readSharedData.get()->getState() == RESOLVE) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_readSharedData.get()->getFd(),
															this->_readSharedData.get()->getType());
		return true;
	}
	return false;
}

void GetResponseBuilder::reset() {
	this->_response.reset();
	this->_sharedData.get()->getBuffer().clear();
}
bool GetResponseBuilder::build() {
	return this->setBody();
}

fd_t GetResponseBuilder::findReadFile() {
	const std::string locPath = "." + this->_locationConfig.get()->getDirectives(ROOT).asString();
	const std::string serverPath = "." + this->_serverConfig.get()->getDirectives(ROOT).asString();
	const std::vector<std::string> indexVec = this->_locationConfig.get()->getDirectives(INDEX).asStrVec();

	for (std::vector<std::string>::const_iterator cit = indexVec.begin(); cit != indexVec.end(); ++cit) {
		this->path = locPath + *cit;
		if (access(this->path.c_str(), R_OK) == 0)
			return utils::makeFd(this->path.c_str(), "r");
	}

	for (std::vector<std::string>::const_iterator cit = indexVec.begin(); cit != indexVec.end(); ++cit) {
		this->path = serverPath + *cit;
		if (access(this->path.c_str(), R_OK) == 0)
			return utils::makeFd(this->path.c_str(), "r");
	}
	return -1;
}

void GetResponseBuilder::prepare() {
	this->_fd = this->findReadFile();
	if (this->_fd == -1)
		throw std::runtime_error("file open error");  // throw ErrorResponseBuilder(404)  404 not found
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr();
	this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().begin(), raw.begin(), raw.end());
	// if (this->_locationConfig.get()->isCgi())
	// 	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::PipeReadHandlerFactory>(this->_readSharedData);
	// else {
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(_fd, EVENT_READ, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
	// }
}