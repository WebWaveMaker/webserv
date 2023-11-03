#pragma once
#ifndef SERVERREADHANDLER_HPP
#define SERVERREADHANDLER_HPP

#include "ServerReadHandler.h"

namespace reactor {

	class ServerReadHandler : public AEventHandler {
	   private:
	   public:
		ServerReadHandler(sharedData_t sharedData);
		~ServerReadHandler();
		virtual void handleEvent();
	};

}  // namespace reactor
#endif
