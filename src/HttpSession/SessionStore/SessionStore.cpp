#include "SessionStore.hpp"

SessionStore::SessionStore() : _sessions() {}
SessionStore::~SessionStore() {}

SessionData& SessionStore::getSession(const std::string& sessionId) {
	if (this->hasSession(sessionId) == false)
		throw std::runtime_error("Session not found");	// TODO: Create a custom exception
	if (this->isSessionExpired(sessionId) == true) {
		this->removeSession(sessionId);
		throw std::runtime_error("Session expired");  // TODO: Create a custom exception
	}
	return this->_sessions.at(sessionId);
}

void SessionStore::setSession(const std::string& sessionId, SessionData& sessionData) {
	this->_sessions[sessionId] = sessionData;
}

bool SessionStore::hasSession(const std::string& sessionId) {
	return this->_sessions.find(sessionId) != this->_sessions.end();
}

bool SessionStore::isSessionExpired(const std::string& sessionId) {
	return this->_sessions[sessionId].isExpired();
}

void SessionStore::removeSession(const std::string& sessionId) {
	this->_sessions.erase(sessionId);
}