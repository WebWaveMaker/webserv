#pragma once
#ifndef SESSIONDATA_HPP
#define SESSIONDATA_HPP

#include "SessionData.h"

class SessionData {
   private:
	const std::string _user;
	const std::time_t _expiredTime;
	std::map<std::string, std::string> _data;

   public:
	SessionData(const std::time_t& expiredTime);
	SessionData(const SessionData& obj);
	~SessionData();
	SessionData& operator=(const SessionData& obj);
	bool isExpired();
	void setData(const std::string& key, const std::string& value);
	std::string& getData(const std::string& key);
};

#endif
