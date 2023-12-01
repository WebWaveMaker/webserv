#pragma once
#ifndef EXEHANDLERFACTORY_HPP
#define EXEHANDLERFACTORY_HPP

#include "ExeHandlerFactory.h"

namespace reactor {
	class ExeHandlerFactory {
	   protected:
		ExeHandlerFactory(){};

	   public:
		virtual ~ExeHandlerFactory(){};
		virtual utils::shared_ptr<AEventHandler> createExeHandler(sharedData_t& sharedData) = 0;
	};
}  // namespace reactor

#endif
