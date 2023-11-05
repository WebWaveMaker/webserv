#pragma once
#ifndef CLIENTREADHANDLERFACTORY_HPP
#define CLIENTREADHANDLERFACTORY_HPP

#include "ClientReadHandlerFactory.h"

namespace reactor {

	class ClientReadHandlerFactory : public IOhandlerFactory {
	   private:
	   public:
		ClientReadHandlerFactory();
		virtual ~ClientReadHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createIOHandler(sharedData_t& sharedData);
	};

}  // namespace reactor

#endif
