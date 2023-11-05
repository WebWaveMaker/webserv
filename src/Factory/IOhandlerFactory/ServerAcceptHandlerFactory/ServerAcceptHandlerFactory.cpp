#include "ServerAcceptHandlerFactory.hpp"

namespace reactor {
	ServerAcceptHandlerFactory::ServerAcceptHandlerFactory() {}
	ServerAcceptHandlerFactory::~ServerAcceptHandlerFactory() {}

	utils::shared_ptr<AEventHandler> ServerAcceptHandlerFactory::createIOHandler(sharedData_t& sharedData) {
		return utils::static_pointer_cast<AEventHandler, ServerAcceptHandler>(
			utils::shared_ptr<ServerAcceptHandler>(new ServerAcceptHandler(sharedData)));
	}
}  // namespace reactor