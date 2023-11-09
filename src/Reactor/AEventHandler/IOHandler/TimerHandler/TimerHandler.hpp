#pragma once
#ifndef TIMERHANDLER_HPP
#define TIMERHANDLER_HPP

#include "TimerHandler.h"

namespace reactor {
	class TimerHandler : public AEventHandler {
	   private:
		TimerHandler(const TimerHandler& obj);
		TimerHandler& operator=(const TimerHandler& obj);

	   public:
		TimerHandler(sharedData_t& sharedData);
		~TimerHandler();
		virtual void handleEvent();
	};

}  // namespace reactor

#endif
