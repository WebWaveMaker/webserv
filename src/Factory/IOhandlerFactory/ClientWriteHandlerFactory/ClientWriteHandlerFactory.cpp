#include "ClientWriteHandlerFactory.hpp"

namespace reactor {
	ClientWriteHandlerFactory::ClientWriteHandlerFactory() {}
	ClientWriteHandlerFactory::~ClientWriteHandlerFactory() {}

	utils::shared_ptr<AEventHandler> ClientWriteHandlerFactory::createIOHandler(sharedData_t& sharedData) {
		return utils::static_pointer_cast<AEventHandler, ClientWriteHandler>(
			utils::shared_ptr<ClientWriteHandler>(new ClientWriteHandler(sharedData)));
	}
}  // namespace reactor
