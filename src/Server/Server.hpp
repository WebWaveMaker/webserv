#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include "Server.h"

class Server : public ICallback {
   private:
	ServerConfig& _serverConfig;
	int _fd;
	struct sockaddr_in _serverAddr;
	std::map<int, ClientEventHandler*>* _clients;
	ServerEventHandler* _eventHandler;
	AccessLogger* _accessLogger;
	ErrorLogger* _errorLogger;

	Server(const Server& obj);
	Server& operator=(const Server& obj);

	void registerEvent(EventType type);
	void removeClient(int key);

   public:
	Server(ServerConfig& serverConfig);
	virtual void eraseClient(int key);
	virtual Client* createClient(int clientFd, struct sockaddr_in& clientAddr);
	virtual ICallback* getCallback();

	int getFd() const;
	const ServerConfig& getConfig() const;
	const sockaddr_in& getAddr() const;
	ServerEventHandler& getEventHandler() const;
	AccessLogger& getAccessLogger() const;
	ErrorLogger& getErrorLogger() const;

	~Server();
};

#endif
