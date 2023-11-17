#include "ErrorResponseBuilder.hpp"

ErrorResponseBuilder::ErrorResponseBuilder(const int errorCode, reactor::sharedData_t sharedData,
										   const utils::shared_ptr<ServerConfig>& config,
										   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _errorCode(errorCode), _sharedData(sharedData), _serverConfig(config), _locationConfig(locationConfig) {
	this->reset();
	this->prepare();
}
ErrorResponseBuilder::~ErrorResponseBuilder() {
	close(this->_fd);
}

reactor::sharedData_t ErrorResponseBuilder::getProduct() {
	return this->_sharedData;
}

void ErrorResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(this->_errorCode));
}

void ErrorResponseBuilder::setHeader() {
	struct stat fileInfo;

	if (stat(this->_path.c_str(), &fileInfo) == SYSTEMCALL_ERROR) {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}

	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);
	headers["Content-Length"] = utils::lltos(fileInfo.st_size);
	this->_response.setHeaders(headers);
}

bool ErrorResponseBuilder::setBody() {
	if (this->_readSharedData->getBuffer().empty())
		return false;
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().end(),
										  this->_readSharedData->getBuffer().begin(),
										  this->_readSharedData->getBuffer().end());
	this->_readSharedData->getBuffer().clear();
	if (this->_readSharedData->getState() == RESOLVE) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_readSharedData->getFd(),
															this->_readSharedData->getType());
		return true;
	}
	return false;
}

void ErrorResponseBuilder::reset() {
	this->_response.reset();
	if (this->_readSharedData.get())
		this->_readSharedData->getBuffer().clear();
	this->_sharedData->getBuffer().clear();
}

bool ErrorResponseBuilder::build() {
	return this->setBody();
}

fd_t ErrorResponseBuilder::findReadFile() {
	const std::string locPath = "." + this->_locationConfig->getDirectives(ROOT).asString();
	const std::string serverPath = "." + this->_serverConfig->getDirectives(ROOT).asString();
	std::string errorPage = this->_locationConfig->getErrorPage(this->_errorCode);

	this->_path = locPath + errorPage;
	if (access(this->_path.c_str(), R_OK) == 0)
		return utils::makeFd(this->_path.c_str(), "r");
	this->_path = serverPath + errorPage;
	if (access(this->_path.c_str(), R_OK) == 0)
		return utils::makeFd(this->_path.c_str(), "r");
	return FD_ERROR;
}

void ErrorResponseBuilder::prepare() {
	this->_fd = this->findReadFile();
	if (this->_fd == FD_ERROR) {
		this->_errorCode = NOT_FOUND;
		this->_path = "./var/error.html";
		this->_fd = utils::makeFd(this->_path.c_str(), "r");
	}
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr();
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().end(), raw.begin(), raw.end());
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(_fd, EVENT_READ, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
}