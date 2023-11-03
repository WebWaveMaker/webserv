#include "ClientReadHandlerFactory.hpp"

ClientReadHandlerFactory::ClientReadHandlerFactory() {}
ClientReadHandlerFactory::~ClientReadHandlerFactory() {}

utils::shared_ptr<AEventHandler> ClientReadHandlerFactory::createHandler(sharedData_t sharedData) {
	return utils::static_pointer_cast<AEventHandler, ClientReadHandler>(
		utils::shared_ptr<ClientReadHandler>(new ClientReadHandler(sharedData)));
}