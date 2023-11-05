#pragma once
#ifndef IOHANDLERFACTORY_HPP
#define IOHANDLERFACTORY_HPP

#include "IOhandlerFactory.h"

namespace reactor {
	class IOhandlerFactory {
	   protected:
		virtual ~IOhandlerFactory(){};

	   public:
		IOhandlerFactory(){};
		virtual utils::shared_ptr<AEventHandler> createIOHandler(sharedData_t& sharedData) = 0;
	};

}  // namespace reactor
#endif
