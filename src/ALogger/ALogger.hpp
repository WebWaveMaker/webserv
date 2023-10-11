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

	static std::string getCurTime(void);

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

	// static method
	static std::string getCurTime(const char* format);

	// pure method
	virtual std::string makeLogMsg(const std::string& msg, const std::string& func) = 0;
	virtual void log(const std::string& msg, const char* func, const int arg) = 0;
};

#endif