#pragma once
#ifndef SIGNUP_HPP
#define SIGNUP_HPP

#include "SignUp.h"

class PostResponseBuilder::SignUp {
   private:
	void signup();

   public:
	SignUp(PostResponseBuilder* builder);
	~SignUp();
};

#endif
