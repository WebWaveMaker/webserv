#pragma once
#ifndef CLIENTREQUESTHANDLERFACTORY_HPP
#define CLIENTREQUESTHANDLERFACTORY_HPP

#include "ClientRequestHandlerFactory.h"

namespace reactor {
	class ClientRequestHandlerFactory : public ExeHandlerFactory {
	   private:
	   public:
		ClientRequestHandlerFactory();
		~ClientRequestHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createExeHandler(sharedData_t& sharedData, va_list args);
	};
}  // namespace reactor
#endif
