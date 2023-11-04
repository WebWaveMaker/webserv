#include "ServerAcceptHandlerFactory.hpp"

namespace reactor {
	ServerAcceptHandlerFactory::ServerAcceptHandlerFactory() {}
	ServerAcceptHandlerFactory::~ServerAcceptHandlerFactory() {}

	utils::shared_ptr<AEventHandler> ServerAcceptHandlerFactory::createHandler(
		utils::shared_ptr<sharedData> sharedData) {
		return utils::static_pointer_cast<AEventHandler, ServerAcceptHandler>(
			utils::shared_ptr<ServerAcceptHandler>(new ServerAcceptHandler(sharedData)));
	}
}  // namespace reactor