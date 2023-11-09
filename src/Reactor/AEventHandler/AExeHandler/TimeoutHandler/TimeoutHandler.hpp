#pragma once
#ifndef TIMEOUTHANDLER_HPP
#define TIMEOUTHANDLER_HPP

#include "TimeoutHandler.h"

namespace reactor {
	class TimeoutHandler : public AExeHandler {
	   private:
		TimeoutHandler(const TimeoutHandler& obj);
		TimeoutHandler& operator=(const TimeoutHandler& obj);

	   public:
		TimeoutHandler(sharedData_t& sharedData);
		virtual void handleEvent();
		~TimeoutHandler();
	};
}  // namespace reactor

#endif
