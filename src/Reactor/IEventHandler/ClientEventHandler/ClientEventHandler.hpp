#pragma once
#ifndef CLIENTEVENTHANDLER_HPP
#define CLIENTEVENTHANDLER_HPP

#include "ClientEventHandler.h"

class Client;

class ClientEventHandler : public reactor::IEventHandler {
   private:
	handle_t _handleFd;
	Client* _client;
	ClientEventHandler(const ClientEventHandler& obj);
	ClientEventHandler& operator=(const ClientEventHandler& obj);

   public:
	ClientEventHandler(handle_t handleFd, Client* client);
	~ClientEventHandler();

	virtual handle_t getHandle() const;
	virtual void handleRead();
	virtual void handleWrite();
	virtual void handleError();
};

#endif
