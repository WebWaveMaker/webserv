#pragma once
#ifndef IOHANDLERFACTORY_HPP
#define IOHANDLERFACTORY_HPP

#include "IOhandlerFactory.h"

class IOhandlerFactory {
   protected:
	virtual ~IOhandlerFactory();

   public:
	IOhandlerFactory();
	virtual utils::shared_ptr<AEventHandler> createHandler(sharedData_t sharedData) = 0;
};

#endif
