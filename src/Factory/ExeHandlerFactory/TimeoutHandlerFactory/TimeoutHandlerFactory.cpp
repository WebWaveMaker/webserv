#include "TimeoutHandlerFactory.hpp"

namespace reactor {
	TimeoutHandlerFactory::TimeoutHandlerFactory() {}
	TimeoutHandlerFactory::~TimeoutHandlerFactory() {}

	utils::shared_ptr<AEventHandler> TimeoutHandlerFactory::createExeHandler(sharedData_t& sharedData) {
		return utils::static_pointer_cast<AEventHandler, TimeoutHandler>(
			utils::shared_ptr<TimeoutHandler>(new TimeoutHandler(sharedData)));
	}

}  // namespace reactor
