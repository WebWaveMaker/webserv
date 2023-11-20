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

	if (stat(this->_path.c_str(), &fileInfo) == -1) {
		throw std::runtime_error("stat error");	 // 500 internal server error
	}

	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig);
	headers["Content-Length"] = utils::lltos(fileInfo.st_size);
	this->_response.setHeaders(headers);
}

bool ErrorResponseBuilder::setBody() {
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

void ErrorResponseBuilder::reset() {
	this->_response.reset();
	if (this->_readSharedData.get())
		this->_readSharedData.get()->getBuffer().clear();
	this->_sharedData.get()->getBuffer().clear();
}

bool ErrorResponseBuilder::build() {
	if (this->_readSharedData->getState() == TERMINATE) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_readSharedData.get()->getFd(),
															this->_readSharedData.get()->getType());
		this->_sharedData->setState(TERMINATE);
		throw false;
	}
	return this->setBody();
}

fd_t ErrorResponseBuilder::findReadFile() {
	// const std::string locPath =
	// 	"." + this->_locationConfig.get()->getOwnRoot(ROOT).asString() +  // 나중에 getOwnRoot바뀔 예정.
	// 	this->_locationConfig.get()->getErrorPage(this->_errorCode);

	return -1;
}

void ErrorResponseBuilder::prepare() {
	this->_fd = this->findReadFile();
	if (this->_fd == -1) {
		this->_errorCode = 404;
		this->_path = "./var/error.html";
		this->_fd = utils::makeFd(this->_path.c_str(), "r");
	}
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr();
	this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().end(), raw.begin(), raw.end());
	if (this->_fd == -1)
		return;
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(_fd, EVENT_READ, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
}