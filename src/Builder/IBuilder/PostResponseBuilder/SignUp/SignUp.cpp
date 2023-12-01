#include "SignUp.hpp"

PostResponseBuilder::SignUp::SignUp(PostResponseBuilder* builder) : _builder(builder) {
	std::string& body = builder->_request->second.getBody();
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
	signup(username, password);
}

PostResponseBuilder::SignUp::~SignUp() {}

void PostResponseBuilder::SignUp::signup(const std::string& username, const std::string& password) {
	if (username == "" || password == "")
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			TEMPORARY_REDIRECT, "/signup.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
	UserDB::getInstance()->addUser(username, password);
	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
		SEE_OTHER, "/login.html", _builder->_sharedData, _builder->_request, _builder->_serverConfig));
}