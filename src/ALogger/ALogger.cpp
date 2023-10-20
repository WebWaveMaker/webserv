#include "ALogger.hpp"

ALogger::ALogger() : fd_(STDOUT_FILENO) {}
ALogger::ALogger(const int fd) : fd_(fd) {}

ALogger::ALogger(const ALogger& obj) : fd_(obj.getFd()) {}

ALogger::~ALogger() {}

ALogger& ALogger::operator=(const ALogger& obj) {
	if (this == &obj)
		return *this;
	const_cast<int&>(fd_) = obj.getFd();
	return *this;
}

int ALogger::getFd(void) const {
	return fd_;
}