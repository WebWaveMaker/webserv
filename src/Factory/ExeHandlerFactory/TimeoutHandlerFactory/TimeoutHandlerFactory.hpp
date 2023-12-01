#pragma once
#ifndef TIMEOUTHANDLERFACTORY_HPP
#define TIMEOUTHANDLERFACTORY_HPP

#include "TimeoutHandlerFactory.h"

namespace reactor {
	class TimeoutHandlerFactory : public ExeHandlerFactory {
	   private:
		TimeoutHandlerFactory(const TimeoutHandlerFactory& obj);
		TimeoutHandlerFactory& operator=(const TimeoutHandlerFactory& obj);

	   public:
		TimeoutHandlerFactory();
		~TimeoutHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createExeHandler(sharedData_t& sharedData);
	};
}  // namespace reactor

#endif
