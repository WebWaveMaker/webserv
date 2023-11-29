#pragma once
#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "Login.h"

class PostResponseBuilder::Login {
   private:
	PostResponseBuilder* _builder;
	HttpSession* _httpSession;

	void login(const std::string& username, const std::string& password);
	bool checkPassword(const std::string& username, const std::string& password);
	void sessionProcess(SessionData& sessionData);
	std::string getSessionCookie(const std::string cookie);

   public:
	Login(PostResponseBuilder* builder);
	~Login();
};

#endif