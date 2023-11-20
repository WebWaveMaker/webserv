#include "SessionData.hpp"

SessionData::SessionData(const std::time_t& expiredTime) : _expiredTime(expiredTime) {}

SessionData::SessionData(const SessionData& obj) : _expiredTime(obj._expiredTime) {}

SessionData::~SessionData() {}

SessionData& SessionData::operator=(const SessionData& obj) {
	if (this == &obj)
		return *this;
	const_cast<std::time_t&>(this->_expiredTime) = obj._expiredTime;
	return *this;
}

bool SessionData::isExpired() {
	if (std::time(u::nullptr_t) > this->_expiredTime) {
		return true;
	}
	return false;
}

void SessionData::setData(const std::string& key, const std::string& value) {
	this->_data[key] = value;
}

std::string& SessionData::getData(const std::string& key) {
	return this->_data[key];
}