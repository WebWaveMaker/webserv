#include "Login.hpp"

PostResponseBuilder::Login::Login(PostResponseBuilder* builder) : _builder(builder) {
	std::string& body = _builder->_request->second.getBody();
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
	login(username, password);
}

PostResponseBuilder::Login::~Login() {}

bool PostResponseBuilder::Login::checkPassword(const std::string& username, const std::string& password) {
	const fd_t fd = utils::makeFd((_folderPath + "/" + username).c_str(), "r");
	char buf[FT];
	ssize_t readBytes;

	if ((readBytes = read(fd, buf, FT)) == SYSTEMCALL_ERROR)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, _builder->_sharedData, _builder->_serverConfig, _builder->_locationConfig));
	buf[readBytes] = '\0';
	close(fd);
	if (password == buf)
		return true;
	return false;
}

void PostResponseBuilder::Login::login(const std::string& username, const std::string& password) {
	if (username == "" || password == "")
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			TEMPORARY_REDIRECT, "/login.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
	if (_builder->findUser(username) == false)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			FOUND, "/login.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
	if (this->checkPassword(username, password) == false)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			FOUND, "/login.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
		SEE_OTHER, "/index.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
}