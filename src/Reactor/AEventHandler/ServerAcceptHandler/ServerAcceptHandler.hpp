#pragma once
#ifndef SERVERACCEPTHANDLER_HPP
#define SERVERACCEPTHANDLER_HPP

#include "ServerAcceptHandler.h"

class ServerAcceptHandler : public reactor::AEventHandler {
   private:
	ICallback* _callback;
	utils::shared_ptr<std::map<int, utils::shared_ptr<Client> > > _clients;

	ServerAcceptHandler(const ServerAcceptHandler& obj);
	ServerAcceptHandler& operator=(const ServerAcceptHandler& obj);

   public:
	ServerAcceptHandler(handle_t fd, ICallback* callback,
						utils::shared_ptr<std::map<int, utils::shared_ptr<Client> > >& clients,
						utils::shared_ptr<AccessLogger>& accessLogger, utils::shared_ptr<ErrorLogger>& errorLogger);

	virtual handle_t getHandle() const;
	virtual void handleEvent();

	~ServerAcceptHandler();
};

#endif
