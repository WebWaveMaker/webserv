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
		virtual utils::shared_ptr<AEventHandler> createHandler(sharedData_t& sharedData, va_list args) = 0;
	};
}  // namespace reactor

#endif
