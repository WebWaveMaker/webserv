#pragma once
#ifndef PIPEREADHANDLERFACTORY_HPP
#define PIPEREADHANDLERFACTORY_HPP

#include "PipeReadHandlerFactory.h"

namespace reactor {

	class PipeReadHandlerFactory : public IOhandlerFactory {
	   private:
	   public:
		PipeReadHandlerFactory();
		virtual ~PipeReadHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createIOHandler(sharedData_t& sharedData);
	};

}  // namespace reactor
#endif
