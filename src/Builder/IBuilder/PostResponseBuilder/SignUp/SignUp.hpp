#pragma once
#ifndef SIGNUP_HPP
#define SIGNUP_HPP

#include "SignUp.h"

class PostResponseBuilder::SignUp {
   private:
	static const std::string _folderPath;
	static const std::string _fileForSignup;

	PostResponseBuilder* _builder;

	static void cleanUpFolder(int sig);

	void signup(const std::string& username, const std::string& password);
	bool findUser(const std::string& username);
	void createUser(const std::string& username, const std::string& password);

   public:
	SignUp(PostResponseBuilder* builder);
	~SignUp();
};

#endif
