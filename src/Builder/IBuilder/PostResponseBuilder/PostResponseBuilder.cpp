#include "PostResponseBuilder.hpp"
#include "Login.hpp"
#include "SignUp.hpp"

const std::string PostResponseBuilder::_folderPath = ".users";
const std::string PostResponseBuilder::_fileForSignup = "for_users";

PostResponseBuilder::PostResponseBuilder(reactor::sharedData_t sharedData, request_t request,
										 const utils::shared_ptr<ServerConfig>& serverConfig,
										 const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _writeSharedData(),
	  _isExist(false),
	  _isRemoved(false),
	  _response(),
	  _path() {
	this->prepare();
};

PostResponseBuilder::~PostResponseBuilder() {
	reactor::FileCloseManager::getInstance()->closeFd(this->_fd);
}

reactor::sharedData_t PostResponseBuilder::getProduct() {
	return this->_sharedData;
}

void PostResponseBuilder::setPath(const std::string& target, const std::string targetPath) {
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

bool PostResponseBuilder::findUser(const std::string& username) {
	DIR* dirp;
	struct dirent* dp;

	if ((dirp = opendir(_folderPath.c_str())) == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
	while ((dp = readdir(dirp)) != u::nullptr_t) {
		if (dp->d_name[0] == '.' || dp->d_name == _fileForSignup)
			continue;
		if (dp->d_name == username) {
			closedir(dirp);
			return true;
		}
	}
	closedir(dirp);
	return false;
}

void PostResponseBuilder::doDefaultBehavior() {}

void PostResponseBuilder::divideEntryPoint() {
	std::string targetTarget = this->_request->second.getRequestTarget();

	if (targetTarget == "/signup")
		SignUp(this);
	if (targetTarget == "/login")
		Login(this);
}

void PostResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(_isExist ? OK : CREATED));
}
void PostResponseBuilder::setHeader() {
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);

	headers.erase("Content-Type");
	headers["Content-Length"] = "0";
	this->_response.setHeaders(headers);
}

bool PostResponseBuilder::setBody() {
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

void PostResponseBuilder::reset() {
	this->_response.reset();
}

bool PostResponseBuilder::build() {
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

void PostResponseBuilder::prepare() {
	this->divideEntryPoint();
	const std::string& target = this->_request->second.getRequestTarget();
	if (target[target.size() - 1] == '/')
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			UNSUPPORTED_MEDIA_TYPE, this->_sharedData, this->_serverConfig, this->_locationConfig));
	// this->setPath(target.substr(1),
	// 			  utils::removeSubstring(this->_request->second.getRequestTarget(), this->_locationConfig->getPath()));
	this->setPath(target.substr(1), this->_request->second.getTargetPath().substr(1));
	if (checkFileMode(this->_path) == MODE_FILE)
		this->_isExist = true;
	else
		this->_isExist = false;
	this->_fd = reactor::FileCloseManager::getInstance()->makeFd(this->_path, "w");
	this->_writeSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(_fd, EVENT_WRITE, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileWriteHandlerFactory>(this->_writeSharedData);
}
