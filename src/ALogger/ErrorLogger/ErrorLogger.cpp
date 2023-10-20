#include "ErrorLogger.hpp"

std::string ErrorLogger::convertLogLevelToString(const enum LogLevel& logLevel) {
	switch (logLevel) {
		case LOG_ERROR:
			return "error";
			break;
		case LOG_WARN:
			return "warn";
			break;
		case LOG_INFO:
			return "info";
			break;
		case LOG_DEBUG:
			return "debug";
			break;

		default:
			break;
	}
}

ErrorLogger::ErrorLogger(const int fd, enum LogLevel logLevel)
	: ALogger(fd), _logLevel(logLevel), _logLevelStr(this->convertLogLevelToString(logLevel)) {}

ErrorLogger::ErrorLogger(const ErrorLogger& obj)
	: ALogger(obj), _logLevel(obj.getLogLevel()), _logLevelStr(obj.getLogLevelStr()) {}

ErrorLogger::~ErrorLogger() {}

ErrorLogger& ErrorLogger::operator=(const ErrorLogger& obj) {
	if (this == &obj)
		return *this;
	this->ALogger::operator=(obj);
	const_cast<enum LogLevel&>(_logLevel) = obj.getLogLevel();
	const_cast<std::string&>(_logLevelStr) = obj.getLogLevelStr();
	return *this;
}

enum LogLevel ErrorLogger::getLogLevel(void) const {
	return this->_logLevel;
}
std::string ErrorLogger::getLogLevelStr(void) const {
	return this->_logLevelStr;
}

std::string ErrorLogger::makeLogMsg(const char* file, const int line, const char* func, const std::string& msg) {
	return utils::getCurTime(logTimeFormat::systemTimeFormat) + " " + static_cast<std::string>(file) + ":" +
		   utils::itos(line) + ": " + RED + "error: " RESET + msg + " reported by " GREEN +
		   static_cast<std::string>(func) + RESET + " function ";
}

/**
 * @brief 
 * 
 * @param file __FILE__ 
 * @param line __LINE__
 * @param func __func__
 */
void ErrorLogger::systemCallError(const char* file, const int line, const char* func) {
	std::cerr << ErrorLogger::makeLogMsg(file, line, func, std::strerror(errno)) << std::endl;
}

/**
 * @brief 
 * 
 * @param file __FILE__ 
 * @param line __LINE__
 * @param func __func__
 * @param msg  your own message on error
 */
void ErrorLogger::systemCallError(const char* file, const int line, const char* func, const std::string& msg) {
	std::cerr << ErrorLogger::makeLogMsg(file, line, func, msg) << std::endl;
}

std::string ErrorLogger::makeLogMsg(const std::string& msg, const std::string& func, void* arg) {
	(void)arg;	// for extra value;
	return utils::getCurTime(logTimeFormat::errorTimeFormat) + RED + "[" + this->_logLevelStr + "] " + RESET + func +
		   "() failed " + "(" + msg + ")\n";
}

// logLevel에 따라 logLevel을 다르게 출력.
void ErrorLogger::log(const std::string& msg, const char* func, const int enum__, void* arg) {
	if (this->_logLevel != enum__)
		return;
	const std::string buf = this->makeLogMsg(msg, func, arg);

	if (write(this->fd_, buf.c_str(), buf.size()) == -1)
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
}
