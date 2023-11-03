#include "ClientWriteHandlerFactory.hpp"

ClientWriteHandlerFactory::ClientWriteHandlerFactory() {}
ClientWriteHandlerFactory::~ClientWriteHandlerFactory() {}

utils::shared_ptr<AEventHandler> ClientWriteHandlerFactory::createHandler(sharedData_t sharedData) {
	return utils::static_pointer_cast<AEventHandler, ClientWriteHandler>(
		utils::shared_ptr<ClientWriteHandler>(new ClientWriteHandler(sharedData)));
}