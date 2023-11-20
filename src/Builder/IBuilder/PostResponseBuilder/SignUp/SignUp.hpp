#pragma once
#ifndef SIGNUP_HPP
#define SIGNUP_HPP

#include "SignUp.h"

class PostResponseBuilder::SignUp {
   private:
	PostResponseBuilder* _builder;

	static void cleanUpFolder(int sig);

	void signup(const std::string& username, const std::string& password);
	void createUser(const std::string& username, const std::string& password);

   public:
	SignUp(PostResponseBuilder* builder);
	~SignUp();
};

#endif
