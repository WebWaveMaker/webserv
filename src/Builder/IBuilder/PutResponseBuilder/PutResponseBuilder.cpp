#include "PutResponseBuilder.hpp"

PutResponseBuilder::PutResponseBuilder(reactor::sharedData_t sharedData, request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig,
									   SessionData* sessionData)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _writeSharedData(),
	  _isExist(false),
	  _isRemoved(false),
	  _response(),
	  _path(),
	  _sessionData(sessionData) {
	this->prepare();
}

PutResponseBuilder::~PutResponseBuilder() {
	close(this->_fd);
}

void PutResponseBuilder::setPath(const std::string& target, const std::string targetPath) {
	const std::string locPath = this->_locationConfig->getDirectives(ROOT).asString() + targetPath;
	const std::string serverPath = this->_serverConfig->getDirectives(ROOT).asString() + targetPath;

	if (access(locPath.c_str(), F_OK) == 0) {
		this->_path = locPath + target;
	} else if (access(serverPath.c_str(), F_OK) == 0) {
		this->_path = serverPath + target;
	} else {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
}

reactor::sharedData_t PutResponseBuilder::getProduct() {
	return this->_sharedData;
}

void PutResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(_isExist ? OK : CREATED));
}

void PutResponseBuilder::setHeader() {
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);

	headers.erase("Content-Type");
	headers["Content-Length"] = "0";
	this->_response.setHeaders(headers);
}

bool PutResponseBuilder::setBody() {
	this->_writeSharedData->getBuffer().insert(this->_writeSharedData->getBuffer().end(),
											   this->_request->second.getBody().begin(),
											   this->_request->second.getBody().end());
	this->_request->second.getBody().clear();
	if ((this->_request->first == DONE || this->_request->first == LONG_BODY_DONE) &&
		this->_writeSharedData->getBuffer().empty() && _isRemoved == false) {
		this->_writeSharedData->setState(RESOLVE);
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_writeSharedData->getFd(),
															this->_writeSharedData->getType());
		this->setStartLine();
		this->setHeader();
		const std::string raw = this->_response.getRawStr();
		this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
		this->_request.get()->second.setContentLength(0);
		this->_request.get()->second.setContentLengthReceived(0);
		return true;
	}
	return false;
}

bool PutResponseBuilder::build() {
	if (this->_writeSharedData.get() == u::nullptr_t)
		return false;
	if (this->_writeSharedData->getState() == TERMINATE) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_writeSharedData->getFd(),
															this->_writeSharedData->getType());
		this->_sharedData->setState(TERMINATE);
		return false;
	}
	if ((this->_request->first == HTTP_ERROR || this->_request->first == LONG_BODY_ERROR ||
		 this->_request->first == CHUNKED_ERROR) &&
		_isRemoved == false) {
		std::remove(this->_path.c_str());
		_isRemoved = true;
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_writeSharedData->getFd(),
															this->_writeSharedData->getType());
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			this->_request->second.getErrorCode(), this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	return this->setBody();
}

void PutResponseBuilder::reset() {
	this->_response.reset();
}

void PutResponseBuilder::prepare() {
	const std::string& target = this->_request->second.getRequestTarget();
	if (target[target.size() - 1] == '/')
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			UNSUPPORTED_MEDIA_TYPE, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->setPath(target.substr(1), this->_request->second.getTargetPath().substr(1));
	if (checkFileMode(this->_path) == MODE_FILE)
		this->_isExist = true;
	else
		this->_isExist = false;
	this->_fd = utils::makeFd(this->_path.c_str(), "w");
	this->_writeSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(_fd, EVENT_WRITE, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileWriteHandlerFactory>(this->_writeSharedData);
}

utils::shared_ptr<IBuilder<reactor::sharedData_t> > PutResponseBuilder::createPutResponseBuilder(
	const reactor::sharedData_t& sharedData, const request_t& request,
	const utils::shared_ptr<ServerConfig>& serverConfig, const utils::shared_ptr<LocationConfig>& locationConfig,
	SessionData* sessionData) {
	return utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
		new PutResponseBuilder(sharedData, request, serverConfig, locationConfig, sessionData));
}
