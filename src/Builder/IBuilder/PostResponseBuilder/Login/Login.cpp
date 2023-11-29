#include "Login.hpp"

PostResponseBuilder::Login::Login(PostResponseBuilder* builder)
	: _builder(builder), _httpSession(HttpSession::getInstance()) {
	const std::string sessionCookie = getSessionCookie(_builder->_request->second.getHeaders()[COOKIE]);
	if (_httpSession->hasSession(sessionCookie))
		sessionProcess(_httpSession->getSessionData(sessionCookie));
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

void PostResponseBuilder::Login::sessionProcess(SessionData& sessionData) {
	(void)sessionData;
	// std::cerr << "sessionProcess" << sessionData.getData("username") << sessionData.getData("password") << std::endl;

	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
		SEE_OTHER, "/index.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
}

std::string PostResponseBuilder::Login::getSessionCookie(const std::string cookie) {
	if (cookie == "")
		return "";
	std::vector<std::string> vec = utils::split(cookie, ";");

	for (std::vector<std::string>::const_iterator cit = vec.begin(); cit != vec.end(); ++cit) {
		std::vector<std::string> vec2 = utils::split(*cit, "=");
		if (vec2.size() != 2)
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
				BAD_REQUEST, _builder->_sharedData, _builder->_serverConfig, _builder->_locationConfig));
		if (vec2[0] == "sessionId")
			return vec2[1];
	}
	return "";
}

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
	const sessionId_t sessionId = _httpSession->createSession(SESSION_EXPIRED_TIME + std::time(u::nullptr_t));
	SessionData& data = _httpSession->getSessionData(sessionId);
	data.setData("username", username);
	data.setData("password", password);
	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
		SEE_OTHER, "/index.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig, sessionId));
}