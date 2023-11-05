#pragma once
#ifndef SERVERACCEPTHANDLERFACTORY_HPP
#define SERVERACCEPTHANDLERFACTORY_HPP

#include "ServerAcceptHandlerFactory.h"

namespace reactor {
	class ServerAcceptHandlerFactory : public ExeHandlerFactory {
	   private:
	   public:
		ServerAcceptHandlerFactory();
		~ServerAcceptHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createHandler(sharedData_t& sharedData, va_list args);
	};

}  // namespace reactor
#endif
