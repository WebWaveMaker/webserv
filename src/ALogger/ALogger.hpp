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
	ALogger(const ALogger& obj);
	ALogger& operator=(const ALogger& obj);

   protected:
	const int fd_;
	const std::string &msg_;

   public:
	ALogger(const int fd = STDOUT_FILENO);
	virtual ~ALogger();
	std::string getCurTime(void);
	void log() = 0;
};

#endif