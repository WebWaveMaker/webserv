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
	const std::string _logLevelStr;

   public:
	// OCF
	ErrorLogger(const int fd, enum LogLevel logLevel);
	ErrorLogger(const ErrorLogger& obj);
	virtual ~ErrorLogger();
	ErrorLogger& operator=(const ErrorLogger& obj);

	// override
	virtual std::string makeLogMsg(const std::string& msg, const std::string& func);
	virtual void log(const std::string& msg, const char* func, const int arg);

	// getter
	enum LogLevel getLogLevel(void) const;
	std::string getLogLevelStr(void) const;

	// method
	std::string convertLogLevelToString(const enum LogLevel& logLevel);

	// static method
	static std::string makeLogMsg(const char* file, const int line, const char* func, const std::string& msg);
	static void systemCallError(const char* file, const int line, const char* func, const std::string& msg);
	static void systemCallError(const char* file, const int line, const char* func);
};

#endif
