#pragma once
#ifndef SERVERACCEPTHANDLERFACTORY_HPP
#define SERVERACCEPTHANDLERFACTORY_HPP

#include "ServerAcceptHandlerFactory.h"

class ServerAcceptHandlerFactory : public IOhandlerFactory {
   private:
   public:
	ServerAcceptHandlerFactory();
	~ServerAcceptHandlerFactory();
	virtual utils::shared_ptr<AeventHandler> createHandler(utils::shared_ptr<ServerData> sharedData);
};

#endif
