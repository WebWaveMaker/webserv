#include "ClientRequestHandlerFactory.hpp"

namespace reactor {
	ClientRequestHandlerFactory::ClientRequestHandlerFactory() {}
	ClientRequestHandlerFactory::~ClientRequestHandlerFactory() {}

	utils::shared_ptr<AEventHandler> ClientRequestHandlerFactory::createHandler(sharedData_t sharedData, va_list args) {
		return utils::static_pointer_cast<AEventHandler, ClientRequestHandler>(
			utils::shared_ptr<ClientRequestHandler>(new ClientRequestHandler(sharedData, args)));
	}
}  // namespace reactor
