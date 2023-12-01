#pragma once
#ifndef SESSIONSTORE_HPP
#define SESSIONSTORE_HPP

#include "SessionStore.h"

class SessionStore : public utils::TSingleton<SessionStore> {
   private:
	std::map<std::string, SessionData> _sessions;

   public:
	SessionStore();
	~SessionStore();
	SessionData& getSession(const std::string& sessionId);
	void setSession(const std::string& sessionId, SessionData& sessionData);
	bool hasSession(const std::string& sessionId);
	bool isSessionExpired(const std::string& sessionId);
	void removeSession(const std::string& sessionId);
};

#endif
