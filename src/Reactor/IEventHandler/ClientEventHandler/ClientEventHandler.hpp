#pragma once
#ifndef CLIENTEVENTHANDLER_HPP
#define CLIENTEVENTHANDLER_HPP

#include "ClientEventHandler.h"

class ClientEventHandler : public reactor::IEventHandler {
   private:
	handle_t _handleFd;
	ICallback* _callback;
	Client* _client;
	ClientEventHandler(const ClientEventHandler& obj);
	ClientEventHandler& operator=(const ClientEventHandler& obj);

   public:
	ClientEventHandler(handle_t handleFd, ICallback* callback, Client* client);
	~ClientEventHandler();

	virtual handle_t getHandle() const;
	Client* getClient() const;
	virtual void handleRead();
	virtual void handleWrite();
	virtual void handleError();
};

#endif
