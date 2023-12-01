#include "HttpSession.hpp"

HttpSession::HttpSession() : _sessionStore(SessionStore::getInstance()) {}

HttpSession::~HttpSession() {}

sessionId_t HttpSession::createSession(const std::time_t& expiredTime) {
	const sessionId_t sessionId = utils::generateRandomString();
	SessionData sessionData(expiredTime);
	this->_sessionStore->setSession(sessionId, sessionData);
	return sessionId;
}

SessionData* HttpSession::getSessionData(const sessionId_t& sessionId) {
	if (hasSession(sessionId) == false)
		return u::nullptr_t;
	return &this->_sessionStore->getSession(sessionId);
}

bool HttpSession::hasSession(const sessionId_t& sessionId) {
	if (sessionId == "")
		return false;
	return this->_sessionStore->hasSession(sessionId);
}

void HttpSession::removeSession(const sessionId_t& sessionId) {
	if (hasSession(sessionId) == false)
		return;
	this->_sessionStore->removeSession(sessionId);
}