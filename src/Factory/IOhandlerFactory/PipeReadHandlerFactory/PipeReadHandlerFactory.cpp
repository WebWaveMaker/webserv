#include "PipeReadHandlerFactory.hpp"

namespace reactor {
	PipeReadHandlerFactory::PipeReadHandlerFactory() {}
	PipeReadHandlerFactory::~PipeReadHandlerFactory() {}

	utils::shared_ptr<AEventHandler> PipeReadHandlerFactory::createIOHandler(sharedData_t& sharedData) {
		return utils::static_pointer_cast<AEventHandler, PipeReadHandler>(
			utils::shared_ptr<PipeReadHandler>(new PipeReadHandler(sharedData)));
	}

}  // namespace reactor