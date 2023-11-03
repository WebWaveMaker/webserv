#pragma once
#ifndef CLIENTREADHANDLER_HPP
#define CLIENTREADHANDLER_HPP

#include "ClientReadHandler.h"

namespace reactor {

	class ClientReadHandler : public AEventHandler {
	   private:
		utils::shared_ptr<RequestParser> _req;

		ClientReadHandler(const ClientReadHandler& obj);
		ClientReadHandler& operator=(const ClientReadHandler& obj);

	   public:
		~ClientReadHandler();
		ClientReadHandler(sharedData_t sharedData, utils::shared_ptr<RequestParser> req);
		virtual void handleEvent();
	};
}  // namespace reactor

#endif
