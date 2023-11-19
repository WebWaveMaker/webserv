#include "SignUp.hpp"

const std::string PostResponseBuilder::SignUp::_folderPath = ".signup";
const std::string PostResponseBuilder::SignUp::_fileForSignup = "for_signup";

void PostResponseBuilder::SignUp::cleanUpFolder(int sig) {
	if (sig != SIGINT)
		return;
	DIR* dirp = opendir(_folderPath.c_str());
	struct dirent* dp;
	while ((dp = readdir(dirp)) != NULL) {
		if (dp->d_name[0] == '.' || dp->d_name == _fileForSignup)
			continue;
		std::string path = _folderPath + "/" + dp->d_name;
		remove(path.c_str());
	}
	closedir(dirp);
	exit(sig);
}

PostResponseBuilder::SignUp::SignUp(PostResponseBuilder* builder) : _builder(builder) {
	signal(SIGINT, cleanUpFolder);
	std::string& body = builder->_request->second.getBody();
	std::string username;
	std::string password;

	std::vector<std::string> vec = utils::split(body, "&");

	for (std::vector<std::string>::const_iterator cit = vec.begin(); cit != vec.end(); ++cit) {
		std::vector<std::string> vec2 = utils::split(*cit, "=");
		if (vec2.size() != 2)
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
				BAD_REQUEST, builder->_sharedData, builder->_serverConfig, builder->_locationConfig));
		if (vec2[0] == "username")
			username = vec2[1];
		else if (vec2[0] == "password")
			password = vec2[1];
	}
	signup(username, password);
}

PostResponseBuilder::SignUp::~SignUp() {}

bool PostResponseBuilder::SignUp::findUser(const std::string& username) {
	DIR* dirp;
	struct dirent* dp;

	if ((dirp = opendir(_folderPath.c_str())) == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, _builder->_sharedData, _builder->_serverConfig, _builder->_locationConfig));
	dp = readdir(dirp);	 // skip current directory
	dp = readdir(dirp);	 // skip parent directory
	dp = readdir(dirp);	 // skip for_signup
	if (dp == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, _builder->_sharedData, _builder->_serverConfig, _builder->_locationConfig));
	while ((dp = readdir(dirp)) != u::nullptr_t) {
		if (dp->d_name == username) {
			closedir(dirp);
			return true;
		}
	}
	closedir(dirp);
	return false;
}

void PostResponseBuilder::SignUp::createUser(const std::string& username, const std::string& password) {
	const fd_t fd = utils::makeFd((_folderPath + "/" + username).c_str(), "w");

	if (write(fd, password.c_str(), password.size()) == SYSTEMCALL_ERROR)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, _builder->_sharedData, _builder->_serverConfig, _builder->_locationConfig));
	close(fd);
}

void PostResponseBuilder::SignUp::signup(const std::string& username, const std::string& password) {
	if (this->findUser(username))
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			FOUND, "/signup.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
	this->createUser(username, password);
	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
		SEE_OTHER, "/login.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
}