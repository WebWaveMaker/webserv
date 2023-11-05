#pragma once
#ifndef CLIENTWRITEHANDLERFACTORY_HPP
#define CLIENTWRITEHANDLERFACTORY_HPP

#include "ClientWriteHandlerFactory.h"

namespace reactor {
	class ClientWriteHandlerFactory : public IOhandlerFactory {
	   private:
	   public:
		ClientWriteHandlerFactory();
		virtual ~ClientWriteHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createHandler(sharedData_t& sharedData);
	};
}  // namespace reactor
#endif
