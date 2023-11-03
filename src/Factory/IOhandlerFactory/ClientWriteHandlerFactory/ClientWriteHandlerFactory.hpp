#pragma once
#ifndef CLIENTWRITEHANDLERFACTORY_HPP
#define CLIENTWRITEHANDLERFACTORY_HPP

#include "ClientReadHandlerFactory.hpp"

class ClientWriteHandlerFactory : public IOhandlerFactory {
   private:
   public:
	ClientWriteHandlerFactory();
	virtual ~ClientWriteHandlerFactory();
	virtual utils::shared_ptr<AEventHandler> createHandler(sharedData_t sharedData);
};

#endif
