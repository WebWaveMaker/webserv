#pragma once
#ifndef CLIENTWRITEHANDLER_HPP
#define CLIENTWRITEHANDLER_HPP

#include "ClientWriteHandler.h"

namespace reactor {
	class ClientWriteHandler : public AEventHandler {
	   private:
		ClientWriteHandler& operator=(const ClientWriteHandler& obj);
		ClientWriteHandler(const ClientWriteHandler& obj);
		ssize_t _idx;

	   public:
		ClientWriteHandler(sharedData_t sharedData);
		~ClientWriteHandler();
		virtual void handleEvent();
	};

}  // namespace reactor

#endif