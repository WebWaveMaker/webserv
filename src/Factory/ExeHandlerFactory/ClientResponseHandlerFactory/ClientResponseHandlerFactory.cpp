#include "ClientResponseHandlerFactory.hpp"

namespace reactor {

	ClientResponseHandlerFactory::ClientResponseHandlerFactory() {}
	ClientResponseHandlerFactory::~ClientResponseHandlerFactory() {}

	utils::shared_ptr<AEventHandler> ClientResponseHandlerFactory::createHandler(sharedData_t& sharedData,
																				 va_list args) {
		return utils::shared_ptr<AEventHandler>(new ClientResponseHandler(sharedData, args));
	}

}  // namespace reactor