#include "ALogger.hpp"

ALogger::ALogger(const std::string& msg, const int fd = STDOUT_FILENO) : msg_(msg), fd_(fd) {}

ALogger::ALogger(const ALogger& obj) : msg_(obj.getMsg()), fd_(obj.getFd()) {}

ALogger::~ALogger() {}

ALogger& ALogger::operator=(const ALogger& obj) {
	if (this == &obj)
		return *this;
	const_cast<int&>(fd_) = obj.getFd();
	const_cast<std::string&>(msg_) = obj.getMsg();
	return *this;
}

std::string ALogger::getCurTime(void) {
	const std::time_t t = std::time(NULL);
	const std::tm* localTime = std::localtime(&t);

	char buf[42];
	std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S", localTime);

	return std::string(buf);
}

std::string ALogger::getMsg(void) const {
	return msg_;
}

int ALogger::getFd(void) const {
	return fd_;
}