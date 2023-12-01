#pragma once
#ifndef SERVERREADHANDLER_HPP
#define SERVERREADHANDLER_HPP

#include "ServerReadHandler.h"

namespace reactor {

	class ServerReadHandler : public AEventHandler {
	   private:
		ServerReadHandler(const ServerReadHandler& obj);
		ServerReadHandler& operator=(const ServerReadHandler& obj);

	   public:
		ServerReadHandler(sharedData_t& sharedData);
		~ServerReadHandler();
		virtual void handleEvent();
	};

}  // namespace reactor
#endif
