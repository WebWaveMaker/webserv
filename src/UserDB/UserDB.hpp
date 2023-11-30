#pragma once
#ifndef USERDB_HPP
#define USERDB_HPP

#include "UserDB.h"

class UserDB : public utils::TSingleton<UserDB> {
   private:
	std::map<std::string, std::string> _userDB;

   public:
	UserDB();
	UserDB(const UserDB& obj);
	~UserDB();
	UserDB& operator=(const UserDB& obj);
	bool hasUser(const std::string& username);
	void addUser(const std::string& username, const std::string& password);
	bool checkPassword(const std::string& username, const std::string& password);
};

#endif
