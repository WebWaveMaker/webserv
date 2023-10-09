#pragma once
#ifndef ERRORLOGGER_HPP
#define ERRORLOGGER_HPP

#include "ErrorLogger.h"

/**
 * @brief 
 * 
 * @param _logLevel // ERROR, WARN, INFO, DEBUG
 * 
 */
class ErrorLogger : public ALogger {
   private:
	const enum LogLevel _logLevel;

   public:
	ErrorLogger(const std::string& msg, const int fd, enum LogLevel logLevel);
	ErrorLogger(const ErrorLogger& obj);
	~ErrorLogger();
	ErrorLogger& operator=(const ErrorLogger& obj);
	virtual void log();
	enum LogLevel getLogLevel(void) const;
};

#endif
