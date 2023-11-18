#pragma once
#ifndef CHILDPROCESSHANDLER_HPP
#define CHILDPROCESSHANDLER_HPP

#include "ChildProcessHandler.h"

namespace reactor {
	class ChildProcessHandler : public AEventHandler {
	   private:
		ChildProcessHandler(const ChildProcessHandler& obj);
		ChildProcessHandler& operator=(const ChildProcessHandler& obj);

	   public:
		ChildProcessHandler(sharedData_t& sharedData);
		~ChildProcessHandler();
		virtual void handleEvent();
	};
}  // namespace reactor

#endif
