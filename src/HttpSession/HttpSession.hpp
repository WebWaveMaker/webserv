#pragma once
#ifndef HTTPSESSION_HPP
#define HTTPSESSION_HPP

#include "HttpSession.h"

class HttpSession : public utils::TSingleton<HttpSession> {
   private:
	SessionStore* _sessionStore;

   public:
	HttpSession();
	~HttpSession();
	sessionId_t createSession(const std::time_t& expiredTime);
	SessionData* getSessionData(const sessionId_t& sessionId);
	bool hasSession(const sessionId_t& sessionId);
};

#endif