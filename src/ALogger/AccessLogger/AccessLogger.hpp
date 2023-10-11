#pragma once
#ifndef ACCESSLOGGER_HPP
#define ACCESSLOGGER_HPP

#include "AccessLogger.h"

class AccessLogger: public ALogger {
   private:

	std::string getHttpMethodStr(const enum &HttpMethod method);



   public:
	AccessLogger();
	AccessLogger(const AccessLogger& obj);
	~AccessLogger();
	AccessLogger& operator=(const AccessLogger& obj);

	// override
	virtual std::string makeLogMsg(const std::string& msg, const std::string& func);
	virtual void log(const std::string& msg, const char* func, const int arg);
};

#endif

