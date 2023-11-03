#include "ClientRequestHandlerFactory.hpp"

ClientRequestHandlerFactory::ClientRequestHandlerFactory() {}

ClientRequestHandlerFactory::~ClientRequestHandlerFactory() {}

utils::shared_ptr<AEventHandler> ClientRequestHandlerFactory::createHandler(sharedData_t sharedData, va_list args) {
	return utils::static_pointer_cast<AEventHandler, ClientRequestHandler>(
		utils::shared_ptr<AEventHandler>(new ClientRequestHandler(sharedData, args)));
}