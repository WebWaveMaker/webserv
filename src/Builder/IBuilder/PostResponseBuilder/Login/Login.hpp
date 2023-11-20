#pragma once
#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "Login.h"

class PostResponseBuilder::Login {
   private:
	PostResponseBuilder* _builder;	

	void login(const std::string& username, const std::string& password);
	bool checkPassword(const std::string& username, const std::string& password);

   public:
	Login(PostResponseBuilder* builder);
	~Login();
};

#endif