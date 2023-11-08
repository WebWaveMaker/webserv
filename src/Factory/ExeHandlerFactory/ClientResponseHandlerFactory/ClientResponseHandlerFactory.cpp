#include "ClientResponseHandlerFactory.hpp"

namespace reactor {

	ClientResponseHandlerFactory::ClientResponseHandlerFactory() {}
	ClientResponseHandlerFactory::~ClientResponseHandlerFactory() {}

	utils::shared_ptr<AEventHandler> ClientResponseHandlerFactory::createExeHandler(sharedData_t& sharedData,
																					va_list args) {
		return utils::static_pointer_cast<AEventHandler, ClientResponseHandler>(
			utils::shared_ptr<ClientResponseHandler>(new ClientResponseHandler(sharedData, args)));
	}

}  // namespace reactor