#pragma once
#ifndef ALOGGER_HPP
#define ALOGGER_HPP

#include "ALogger.h"
/**
 * @brief 
 * 
 * @param fd_ default value = STDOUT_FILENO
 */
class ALogger {

   private:
   protected:
	const int fd_;

	// pure method
	virtual std::string makeLogMsg(const std::string& msg, const std::string& func, void* arg) = 0;

   public:
	//OCF
	ALogger();
	ALogger(const int fd);
	ALogger(const ALogger& obj);
	ALogger& operator=(const ALogger& obj);
	virtual ~ALogger();

	// getter
	int getFd(void) const;

	// method

	// pure method
	virtual void log(const std::string& msg, const char* func, const int enum__, void* arg) = 0;
};

#endif
