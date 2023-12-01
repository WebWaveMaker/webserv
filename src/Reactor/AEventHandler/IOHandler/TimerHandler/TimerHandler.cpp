#include "TimerHandler.hpp"

namespace reactor {
	TimerHandler::TimerHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}

	void TimerHandler::handleEvent() {
		this->setState(ACCEPT);
	}

	TimerHandler::~TimerHandler() {}
}  // namespace reactor
