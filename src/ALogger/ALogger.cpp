#include "ALogger.hpp"

ALogger::ALogger(const int fd = STDOUT_FILENO) : fd_(fd) {}
ALogger::ALogger(const ALogger& obj) {}
ALogger::~ALogger() {}
ALogger& ALogger::operator=(const ALogger& obj) {}

std::string ALogger::getCurTime(void) {
	const std::time_t t = std::time(NULL);
	std::tm* localTime = std::localtime(&t);

	char buf[1000];
	std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S", timePtr);

    return std::string(buf);
}