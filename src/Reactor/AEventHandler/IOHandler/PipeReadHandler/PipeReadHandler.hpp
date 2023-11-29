#pragma once
#ifndef PIPEREADHANDLER_HPP
#define PIPEREADHANDLER_HPP

#include "PipeReadHandler.h"

namespace reactor {
	class PipeReadHandler : public AEventHandler {
	   private:
	   public:
		PipeReadHandler(sharedData_t& sharedData);
		~PipeReadHandler();
		virtual void handleEvent();
	};
}  // namespace reactor
#endif
