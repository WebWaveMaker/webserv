#include "ErrorLogger.hpp"

ErrorLogger::ErrorLogger(const int fd, enum LogLevel logLevel) : ALogger(fd), _logLevel(logLevel) {}

ErrorLogger::ErrorLogger(const ErrorLogger& obj) : ALogger(obj), _logLevel(obj.getLogLevel()) {}

ErrorLogger::~ErrorLogger() {}

ErrorLogger& ErrorLogger::operator=(const ErrorLogger& obj) {
	if (this == &obj)
		return *this;
	const_cast<enum LogLevel&>(_logLevel) = obj.getLogLevel();
	return *this;
}

enum LogLevel ErrorLogger::getLogLevel(void) const {
	return this->_logLevel;
}

/**
 * @brief 
 * 
 * @param file __FILE__ 
 * @param line __LINE__
 * @param func __func__
 */
void ErrorLogger::systemCallError(const char* file, const int line, const char* func) {
	std::cerr << ALogger::getCurTime() + " " + static_cast<std::string>(file) + ":" + utils::itos(line) + ": " + RED +
					 "error: " RESET + std::strerror(errno) + " reported by " GREEN + static_cast<std::string>(func) +
					 RESET + " function "
			  << std::endl;
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
	std::cerr << ALogger::getCurTime() + " " + static_cast<std::string>(file) + ":" + utils::itos(line) + ": " + RED +
					 "error: " RESET + msg + " reported by " GREEN + static_cast<std::string>(func) + RESET +
					 " function "
			  << std::endl;
}

std::string ErrorLogger::makeLogMsg(const std::string& msg) {
	return msg;
}

// logLevel에 따라 logLevel을 다르게 출력.
void ErrorLogger::log(const std::string& msg, const int arg) {
	if (this->_logLevel != arg)
		return;
	const std::string buf = this->makeLogMsg(msg);

	if (write(this->fd_, buf.c_str(), buf.size()) == -1)
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
}
