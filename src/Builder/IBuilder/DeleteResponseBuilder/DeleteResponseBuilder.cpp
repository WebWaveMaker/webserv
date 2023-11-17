#include "DeleteResponseBuilder.hpp"

DeleteResponseBuilder::DeleteResponseBuilder(reactor::sharedData_t sharedData, request_t request,
											 const utils::shared_ptr<ServerConfig>& serverConfig,
											 const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _readSharedData(new reactor::SharedData(FD_ERROR, EVENT_READ, std::vector<char>())),
	  _response(),
	  _path() {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->prepare();
}

DeleteResponseBuilder::~DeleteResponseBuilder() {}

bool DeleteResponseBuilder::implDeleteFile(const std::string& path) {
	switch (checkFileMode(path)) {
		case MODE_FILE:
			if (std::remove(path.c_str()) == SYSTEMCALL_ERROR)
				throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
					INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
			this->_path = path;
			return true;
		case MODE_DIRECTORY:
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
				new ErrorResponseBuilder(FORBIDDEN, this->_sharedData, this->_serverConfig, this->_locationConfig));
			break;
		default:
			break;
	}
	return false;
}

void DeleteResponseBuilder::deleteFile() {
	const std::string locPath = "." + this->_locationConfig->getDirectives(ROOT).asString() + "/" +
								this->_request->second.getTargetFile();	 // slash는 임시로 추가 이후수정
	const std::string serverPath =
		"." + this->_serverConfig->getDirectives(ROOT).asString() + this->_request->second.getTargetFile();

	if (this->implDeleteFile(locPath))
		return;
	if (this->implDeleteFile(serverPath))
		return;
	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
		new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
}

reactor::sharedData_t DeleteResponseBuilder::getProduct() {
	return this->_sharedData;
}

void DeleteResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(OK));
}

void DeleteResponseBuilder::setHeader() {
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);

	headers.erase("Content-Type");
	headers["Content-Length"] = "0";
	this->_response.setHeaders(headers);
}

bool DeleteResponseBuilder::setBody() {
	if (this->_sharedData.get() == u::nullptr_t)
		return false;
	return true;
}

bool DeleteResponseBuilder::build() {
	return this->setBody();
}

void DeleteResponseBuilder::reset() {
	this->_response.reset();
}

void DeleteResponseBuilder::prepare() {
	this->deleteFile();
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr() + CRLF;
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData->setState(RESOLVE);
}
