#pragma once
#ifndef SERVEREVENTHANDLER_HPP
#define SERVEREVENTHANDLER_HPP

#include "ServerEventHandler.h"

class Client;
class AccessLogger;
class ErrorLogger;

class ServerEventHandler : public reactor::IEventHandler {
   private:
	std::map<int, Client*>* _clients;
	handle_t _handleFd;
	AccessLogger* _accessLogger;
	ErrorLogger* _errorLogger;
	ServerEventHandler& operator=(const ServerEventHandler& obj);
	ServerEventHandler(const ServerEventHandler& obj);

   public:
	ServerEventHandler(handle_t handleFd, std::map<int, Client*>* clients, AccessLogger* accessLogger,
					   ErrorLogger* errorLogger);
	virtual handle_t getHandle() const;
	virtual void handleRead();
	virtual void handleWrite();
	virtual void handleError();
	virtual ~ServerEventHandler();
};

#endif
