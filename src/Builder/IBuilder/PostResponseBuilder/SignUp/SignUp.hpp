#pragma once
#ifndef SIGNUP_HPP
#define SIGNUP_HPP

class SignUp {
   private:
   public:
	SignUp();
	SignUp(const SignUp& obj);
	~SignUp();
	SignUp& operator=(const SignUp& obj);
};

#endif

SignUp::SignUp() {}
SignUp::SignUp(const SignUp& obj) {}
SignUp::~SignUp() {}
SignUp& SignUp::operator=(const SignUp& obj) {}