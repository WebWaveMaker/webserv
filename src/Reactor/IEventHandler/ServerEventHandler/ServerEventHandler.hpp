#pragma once
#ifndef SERVEREVENTHANDLER_HPP
#define SERVEREVENTHANDLER_HPP

#include "ServerEventHandler.h"

class ServerEventHandler : public reactor::IEventHandler {
   private:
	handle_t _handleFd;
	std::map<int, ClientEventHandler*>* _clients;
	ServerConfig& _serverConfig;
	AccessLogger* _accessLogger;
	ErrorLogger* _errorLogger;
	ServerEventHandler& operator=(const ServerEventHandler& obj);
	ServerEventHandler(const ServerEventHandler& obj);

   public:
	ServerEventHandler(handle_t handleFd, std::map<int, ClientEventHandler*>* clients, ServerConfig& serverConfig,
					   AccessLogger* accessLogger, ErrorLogger* errorLogger);
	virtual handle_t getHandle() const;
	virtual void handleRead();
	virtual void handleWrite();
	virtual void handleError();
	virtual ~ServerEventHandler();
};

#endif
