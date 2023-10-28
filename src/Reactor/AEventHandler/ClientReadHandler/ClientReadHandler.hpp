#pragma once
#ifndef CLIENTREADHANDLER_HPP
#define CLIENTREADHANDLER_HPP

#include "ClientReadHandler.h"

namespace reactor {

	class ClientReadHandler : public AEventHandler {
	   private:
		utils::shared_ptr<Client> _client;

		ClientReadHandler(const ClientReadHandler& obj);
		ClientReadHandler& operator=(const ClientReadHandler& obj);

	   public:
		~ClientReadHandler();
		ClientReadHandler(const handle_t fd, const utils::shared_ptr<AccessLogger>& accessLogger,
						  const utils::shared_ptr<ErrorLogger>& errorLogger, const utils::shared_ptr<Client>& client);
		virtual void handleEvent();
	};
}  // namespace reactor

#endif
