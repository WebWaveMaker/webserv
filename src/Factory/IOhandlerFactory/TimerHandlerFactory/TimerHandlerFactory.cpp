#include "TimerHandlerFactory.hpp"

namespace reactor {
	TimerHandlerFactory::TimerHandlerFactory() {}
	TimerHandlerFactory::~TimerHandlerFactory() {}

	utils::shared_ptr<AEventHandler> TimerHandlerFactory::createIOHandler(sharedData_t& sharedData) {
		return utils::static_pointer_cast<AEventHandler, TimerHandler>(
			utils::shared_ptr<TimerHandler>(new TimerHandler(sharedData)));
	}
}  // namespace reactor
