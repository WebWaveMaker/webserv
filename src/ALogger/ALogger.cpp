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

std::string ALogger::getCurTime(const char* format) {
	const std::time_t t = std::time(NULL);
	const std::tm* localTime = std::localtime(&t);

	char buf[42];
	std::strftime(buf, sizeof(buf), format, localTime);

	return DARK_BLUE + std::string(buf) + RESET;
}

int ALogger::getFd(void) const {
	return fd_;
}