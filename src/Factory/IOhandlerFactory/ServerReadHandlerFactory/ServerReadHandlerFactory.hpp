#pragma once
#ifndef SERVERREADHANDLERFACTORY_CPP
#define SERVERREADHANDLERFACTORY_CPP

#include "ServerReadHandlerFactory.h"

namespace reactor {
	class ServerReadHandlerFactory : public IOhandlerFactory {
	   private:
	   public:
		ServerReadHandlerFactory();
		~ServerReadHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createIOHandler(sharedData_t& sharedData);
	};
}  // namespace reactor
#endif
