#pragma once
#ifndef LOGIN_HPP
#define LOGIN_HPP

class Login {
   private:
   public:
	Login();
	Login(const Login& obj);
	~Login();
	Login& operator=(const Login& obj);
};

#endif

Login::Login() {}
Login::Login(const Login& obj) {}
Login::~Login() {}
Login& Login::operator=(const Login& obj) {}