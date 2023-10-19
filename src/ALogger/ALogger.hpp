#pragma once
#ifndef ALOGGER_HPP
#define ALOGGER_HPP

#include "ALogger.h"

/**
 * @brief 
 * 
 * @param fd_ default value = STDOUT_FILENO
 * @param msg_
 */
class ALogger {
   private:
   protected:
	const std::string msg_;
	const int fd_;

   public:
	ALogger(const std::string& msg, const int fd);
	ALogger(const ALogger& obj);
	ALogger& operator=(const ALogger& obj);
	virtual ~ALogger();
	std::string getCurTime(void);
	std::string getMsg(void) const;
	int getFd(void) const;
	virtual void log() = 0;
};

#endif