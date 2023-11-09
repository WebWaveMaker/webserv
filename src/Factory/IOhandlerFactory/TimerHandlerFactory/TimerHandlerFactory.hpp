#pragma once
#ifndef TIMERHANDLERFACTORY_HPP
#define TIMERHANDLERFACTORY_HPP

#include "TimerHandlerFactory.h"

namespace reactor {
	class TimerHandlerFactory : public IOhandlerFactory {
	   private:
		TimerHandlerFactory(const TimerHandlerFactory& obj);
		TimerHandlerFactory& operator=(const TimerHandlerFactory& obj);

	   public:
		TimerHandlerFactory();
		~TimerHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createIOHandler(sharedData_t& sharedData);
	};
}  // namespace reactor

#endif
