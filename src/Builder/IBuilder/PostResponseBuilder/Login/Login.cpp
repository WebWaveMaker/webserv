#include "Login.hpp"

PostResponseBuilder::Login::Login(PostResponseBuilder* builder)
	: _builder(builder), _httpSession(HttpSession::getInstance()), _userDB(UserDB::getInstance()) {
	std::string& body = _builder->_request->second.getBody();
	std::string username;
	std::string password;

	std::vector<std::string> vec = utils::split(body, "&");

	for (std::vector<std::string>::const_iterator cit = vec.begin(); cit != vec.end(); ++cit) {
		std::vector<std::string> vec2 = utils::split(*cit, "=");
		if (vec2.size() != 2)
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
				new ErrorResponseBuilder(BAD_REQUEST, builder->_sharedData, builder->_request, builder->_serverConfig,
										 builder->_locationConfig));
		if (vec2[0] == "username")
			username = vec2[1];
		else if (vec2[0] == "password")
			password = vec2[1];
	}
	login(username, password);
}

PostResponseBuilder::Login::~Login() {}

void PostResponseBuilder::Login::login(const std::string& username, const std::string& password) {
	if (username == "" || password == "")
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			TEMPORARY_REDIRECT, "/login.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
	if (_userDB->hasUser(username) == false)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			FOUND, "/login.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
	if (_userDB->checkPassword(username, password) == false)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			FOUND, "/login.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
	const sessionId_t sessionId = _httpSession->createSession(SESSION_EXPIRED_TIME + std::time(u::nullptr_t));
	SessionData* data = _httpSession->getSessionData(sessionId);
	data->setData("USERNAME", username);
	data->setData("PASSWORD", password);
	data->setData("COLOR", utils::generateRandomHexColor());
	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
		SEE_OTHER, "/index.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig, sessionId));
}
