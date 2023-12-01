#pragma once
#ifndef CLIENTRESPONSEHANDLERFACTORY_HPP
#define CLIENTRESPONSEHANDLERFACTORY_HPP

#include "ClientResponseHandlerFactory.h"

namespace reactor {
	class ClientResponseHandlerFactory : public ExeHandlerFactory {
	   private:
	   public:
		ClientResponseHandlerFactory();
		~ClientResponseHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createExeHandler(sharedData_t& sharedData);
	};
}  // namespace reactor

#endif