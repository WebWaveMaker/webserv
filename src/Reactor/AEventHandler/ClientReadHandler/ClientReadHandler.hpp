#pragma once
#ifndef CLIENTREADHANDLER_HPP
#define CLIENTREADHANDLER_HPP

#include "ClientReadHandler.h"

namespace reactor {

	class ClientReadHandler : public AEventHandler {
	   private:
		ClientReadHandler(const ClientReadHandler& obj);
		ClientReadHandler& operator=(const ClientReadHandler& obj);

	   public:
		~ClientReadHandler();
		ClientReadHandler(sharedData_t sharedData);
		virtual void handleEvent();
	};
}  // namespace reactor

#endif
