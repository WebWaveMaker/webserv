#pragma once
#ifndef SERVERACCEPTHANDLER_HPP
#define SERVERACCEPTHANDLER_HPP

#include "ServerAcceptHandler.h"

namespace reactor {

	class ServerAcceptHandler : public reactor::AEventHandler {
	   private:
		ICallback* _callback;
		u::shared_ptr<std::map<int, u::shared_ptr<Client> > > _clients;

		ServerAcceptHandler(const ServerAcceptHandler& obj);
		ServerAcceptHandler& operator=(const ServerAcceptHandler& obj);

	   public:
		ServerAcceptHandler(handle_t fd, ICallback* callback,
							u::shared_ptr<std::map<int, u::shared_ptr<Client> > >& clients,
							u::shared_ptr<AccessLogger>& accessLogger, u::shared_ptr<ErrorLogger>& errorLogger);

		virtual handle_t getHandle() const;
		virtual void handleEvent();

		~ServerAcceptHandler();
	};

}  // namespace reactor

#endif
