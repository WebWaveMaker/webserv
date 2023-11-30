#include "UserDB.hpp"

UserDB::UserDB() : _userDB() {}

UserDB::~UserDB() {}

UserDB::UserDB(const UserDB& obj) : _userDB(obj._userDB) {}

UserDB& UserDB::operator=(const UserDB& obj) {
	if (this != &obj)
		this->_userDB = obj._userDB;
	return *this;
}

bool UserDB::hasUser(const std::string& username) {
	return this->_userDB.find(username) != this->_userDB.end();
}

void UserDB::addUser(const std::string& username, const std::string& password) {
	this->_userDB[username] = password;
}

bool UserDB::checkPassword(const std::string& username, const std::string& password) {
	if (!this->hasUser(username))
		return false;
	return this->_userDB[username] == password;
}
