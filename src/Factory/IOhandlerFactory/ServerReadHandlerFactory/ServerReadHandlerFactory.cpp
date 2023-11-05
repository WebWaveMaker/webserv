#include "ServerReadHandlerFactory.hpp"

namespace reactor {
	ServerReadHandlerFactory::ServerReadHandlerFactory() {}
	ServerReadHandlerFactory::~ServerReadHandlerFactory() {}
	utils::shared_ptr<AEventHandler> ServerReadHandlerFactory::createHandler(sharedData_t& sharedData) {
		return utils::static_pointer_cast<AEventHandler, ServerReadHandler>(
			utils::shared_ptr<ServerReadHandler>(new ServerReadHandler(sharedData)));
	}

}  // namespace reactor