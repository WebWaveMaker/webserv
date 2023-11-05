#pragma once
#ifndef SERVERACCEPTHANDLER_HPP
#define SERVERACCEPTHANDLER_HPP

#include "ServerAcceptHandler.h"

namespace reactor {

	class ServerAcceptHandler : public AEventHandler {
	   private:
		ServerAcceptHandler(const ServerAcceptHandler& obj);
		ServerAcceptHandler& operator=(const ServerAcceptHandler& obj);

	   public:
		ServerAcceptHandler(sharedData_t& sharedData);
		virtual void handleEvent();
		virtual ~ServerAcceptHandler();
	};

}  // namespace reactor

#endif
