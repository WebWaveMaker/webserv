#include "ServerAcceptHandlerFactory.hpp"

namespace reactor {
	ServerAcceptHandlerFactory::ServerAcceptHandlerFactory() {}
	ServerAcceptHandlerFactory::~ServerAcceptHandlerFactory() {}

	utils::shared_ptr<AEventHandler> ServerAcceptHandlerFactory::createExeHandler(sharedData_t& sharedData,
																				  va_list args) {
		va_end(args);
		return utils::static_pointer_cast<AEventHandler, ServerAcceptHandler>(
			utils::shared_ptr<ServerAcceptHandler>(new ServerAcceptHandler(sharedData)));
	}

}  // namespace reactor