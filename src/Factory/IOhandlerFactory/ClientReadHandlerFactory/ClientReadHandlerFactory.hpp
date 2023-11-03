#pragma once
#ifndef CLIENTREADHANDLERFACTORY_HPP
#define CLIENTREADHANDLERFACTORY_HPP

#include "ClientReadHandlerFactory.h"

class ClientReadHandlerFactory : public IOhandlerFactory {
   private:
   public:
	ClientReadHandlerFactory();
	virtual ~ClientReadHandlerFactory();
	virutal utils::shared_ptr<AEventHandler> createHandler(sharedData_t sharedData);
};

#endif
