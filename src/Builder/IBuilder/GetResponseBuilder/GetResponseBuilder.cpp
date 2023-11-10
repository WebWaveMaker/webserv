#include "GetResponseBuilder.hpp"

GetResponseBuilder::GetResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& config)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(config),
	  _locationConfig(config.get()->getLocationConfig(request.get()->second.getRequestTarget())),
	  _readSharedData() {
	if (_locationConfig.get() == u::nullptr_t)
		throw std::runtime_error("location config is null");  // throw ErrorResponseBuilder(404)  404 not found
	this->prepare();
}

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
	struct stat fileInfo;
	const std::string path = this->_locationConfig.get()->getDirectives(ROOT).asString();

	// cgi 처리는 다르게 해야함.
	if (access(path.c_str(), R_OK) == -1)
		throw std::runtime_error("access error");  // // throw ErrorResponseBuilder(500) internal server error later
	if (stat(path.c_str(), &fileInfo) < 0) {
		throw std::runtime_error("stat error");	 // throw ErrorResponseBuilder(500) internal server error later
	}

	std::map<std::string, std::string> headers;
	headers["Server"] = this->_serverConfig.get()->getDirectives(SERVER_NAME).asString();
	headers["Date"] = utils::getCurTime(logTimeFormat::UTCtimeFormat);
	headers["Content-Type"] = this->_serverConfig.get()->getDirectives(DEFAULT_TYPE).asString();
	headers["Content-Length"] = utils::lltos(fileInfo.st_size);
	this->_response.setHeaders(headers);
}

bool GetResponseBuilder::setBody() {
	this->_sharedData.get()->getBuffer() = this->_readSharedData.get()->getBuffer();
	if (this->_readSharedData.get()->getState() == RESOLVE)
		return true;
	return false;
}

void GetResponseBuilder::reset() {
	this->_response.reset();
	this->_sharedData.get()->getBuffer().clear();
}
bool GetResponseBuilder::build() {
	return this->setBody();
}
void GetResponseBuilder::prepare() {
	const std::string path = this->_locationConfig.get()->getDirectives(ROOT).asString();

	this->setStartLine();
	this->setHeader();
	if (this->_locationConfig.get()->isCgi())
		reactor::Dispatcher::getInstance()->registerIOHandler<reactor::PipeReadHandlerFactory>(this->_readSharedData);
	else {
		this->_readSharedData = utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(
			utils::makeFd(path.c_str(), "r"), EVENT_READ, this->_sharedData.get()->getBuffer()));
		reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
	}
}