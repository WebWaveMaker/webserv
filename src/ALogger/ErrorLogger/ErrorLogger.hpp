#pragma once
#ifndef ERRORLOGGER_HPP
#define ERRORLOGGER_HPP

#include "ErrorLogger.h"

/**
 * @brief 
 * 
 * @param _logLevel // LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG
 * 
 */
class ErrorLogger : public ALogger {
   private:
	const enum LogLevel _logLevel;

   public:
	ErrorLogger(const int fd, enum LogLevel logLevel);
	ErrorLogger(const ErrorLogger& obj);
	virtual ~ErrorLogger();
	ErrorLogger& operator=(const ErrorLogger& obj);

	// override
	virtual std::string makeLogMsg(const std::string& msg);
	virtual void log(const std::string& msg, const int arg);

	// getter
	enum LogLevel getLogLevel(void) const;

	// static method
	static void systemCallError(const char* file, const int line, const char* func, const std::string& msg);
	static void systemCallError(const char* file, const int line, const char* func);
};

#endif
