#pragma once
#ifndef SERVERACCEPTHANDLERFACTORY_HPP
#define SERVERACCEPTHANDLERFACTORY_HPP

#include "ServerAcceptHandlerFactory.h"

namespace reactor {
	class ServerAcceptHandlerFactory : public IOhandlerFactory {
	   private:
	   public:
		ServerAcceptHandlerFactory();
		~ServerAcceptHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createHandler(utils::shared_ptr<sharedData> sharedData);
	};
}  // namespace reactor
#endif
