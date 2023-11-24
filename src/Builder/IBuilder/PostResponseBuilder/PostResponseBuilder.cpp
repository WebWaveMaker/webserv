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
	  _response(),
	  _readSharedData() {
	this->prepare();
};

PostResponseBuilder::~PostResponseBuilder() {}

reactor::sharedData_t PostResponseBuilder::getProduct() {
	return this->_sharedData;
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

	// default 동작은 파일업로드.
	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
		new PutResponseBuilder(this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
}

void PostResponseBuilder::setStartLine() {}
void PostResponseBuilder::setHeader() {}
bool PostResponseBuilder::setBody() {
	return true;
}
void PostResponseBuilder::reset() {}
bool PostResponseBuilder::build() {
	return this->setBody();
}
void PostResponseBuilder::prepare() {
	this->divideEntryPoint();
}
