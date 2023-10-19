#include "ErrorLogger.hpp"

ErrorLogger::ErrorLogger(const std::string& msg, const int fd, enum LogLevel logLevel)
	: ALogger(msg, fd), _logLevel(logLevel) {}

ErrorLogger::ErrorLogger(const ErrorLogger& obj) : ALogger(obj), _logLevel(obj.getLogLevel()) {}

ErrorLogger::~ErrorLogger() {}

ErrorLogger& ErrorLogger::operator=(const ErrorLogger& obj) {
	if (this == &obj)
		return *this;
	const_cast<enum LogLevel&>(_logLevel) = obj.getLogLevel();
	return *this;
	
}

void ErrorLogger::log() {}

enum LogLevel ErrorLogger::getLogLevel(void) const {
	return _logLevel;
}