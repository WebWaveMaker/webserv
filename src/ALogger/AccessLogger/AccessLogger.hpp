#pragma once
#ifndef ACCESSLOGGER_HPP
#define ACCESSLOGGER_HPP

#include "AccessLogger.h"

class AccessLogger : public ALogger {
   private:
	std::string getHttpMethodStr(const enum HttpMethod& method);

	// override
	virtual std::string makeLogMsg(const std::string& msg, const std::string& func, void* arg);

   public:
	// OCF
	AccessLogger(const int fd);
	AccessLogger(const AccessLogger& obj);
	~AccessLogger();
	AccessLogger& operator=(const AccessLogger& obj);

	// override
	virtual void log(const std::string& msg, const char* func, const int enum__, void* arg);
};

#endif
