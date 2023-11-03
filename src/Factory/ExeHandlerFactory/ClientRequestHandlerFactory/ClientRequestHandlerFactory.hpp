#pragma once
#ifndef CLIENTREQUESTHANDLERFACTORY_HPP
#define CLIENTREQUESTHANDLERFACTORY_HPP

#include "ClientRequestHandlerFactory.h"

class ClientRequestHandlerFactory : public ExeHandlerFactory {
   private:
   public:
	ClientRequestHandlerFactory();
	~ClientRequestHandlerFactory();
	virtual utils::shared_ptr<AEventHandler> createHandler(sharedData_t sharedData, va_list args);
};

#endif
