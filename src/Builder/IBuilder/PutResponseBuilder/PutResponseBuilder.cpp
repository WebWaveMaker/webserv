#include "PutResponseBuilder.h"

PutResponseBuilder::PutResponseBuilder(reactor::sharedData_t sharedData, request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _writeSharedData(),
	  _readSharedData(new reactor::SharedData(FD_ERROR, EVENT_READ, std::vector<char>())),
	  _response(),
	  _path() {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->prepare();
}

PutResponseBuilder::~PutResponseBuilder() {}

bool PutResponseBuilder::updateFile() {
	std::vector<char> body = this->_request->second.getBody();
	this->_writeSharedData->get()->getBuffer().insert(this->_writeSharedData->get()->getBuffer().end(), body.begin(),
													  body.end());

	if (this->_sharedData->getState == DONE && this->_writeSharedData->get()->getBuffer().size() == 0) {
		return true;
	}
	return false;
}

void PutResponseBuilder::setPath() {
	const std::string locPath =
		this->_locationConfig->getDirectives(ROOT).asString() + "/" + this->_request->second.getTargetFile();
	const std::string serverPath =
		this->_serverConfig->getDirectives(ROOT).asString() + "/" + this->_request->second.getTargetFile();
	enum FileMode locMode = checkFileMode(locPath);
	enum FileMode serverMode = checkFileMode(serverPath);

	switch (locMode) {
		case MODE_FILE:
			this->_path = locPath;
			break;
		case MODE_DIRECTORY:
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
				new ErrorResponseBuilder(FORBIDDEN, this->_sharedData, this->_serverConfig, this->_locationConfig));
		case MODE_ERROR:
			switch (serverMode) {
				case MODE_FILE:
					this->_path = serverPath;
					break;
				case MODE_DIRECTORY:
					throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
						FORBIDDEN, this->_sharedData, this->_serverConfig, this->_locationConfig));
					break;
				case MODE_ERROR:
					throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
						INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
					break;
			}
			break;
	}
}

reactor::sharedData_t PutResponseBuilder::getProduct() {
	return this->_sharedData;
}

void PutResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(OK));
}

void PutResponseBuilder::setHeader() {
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);

	headers.erase("Content-Type");
	headers["Content-Length"] = "0";
	this->_response.setHeaders(headers);
}

bool PutResponseBuilder::setBody() {
	if (this->_writeSharedData->get()->getState() == TERMINATE) {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
		return false;
	}
	return this->updateFile();
}

bool PutResponseBuilder::build() {
	if (this->setBody() == false)
		return false;
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr() + CRLF;
	std::cerr << raw << std::endl;
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData->setState(RESOLVE);
	this->_writeSharedData->get()->setState(RESOLVE);
	return true;
}

void PutResponseBuilder::reset() {
	this->_response.reset();
}

void PutResponseBuilder::prepare() {
	this->setPath();
	this->_writeSharedData = utils::shared_ptr<reactor::SharedData>(
		new reactor::SharedData(utils::makeFd(this->_path.c_str(), "w"), EVENT_WRITE, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileWriteHandlerFactory>(this->_writeSharedData);
	this->setBody();
}