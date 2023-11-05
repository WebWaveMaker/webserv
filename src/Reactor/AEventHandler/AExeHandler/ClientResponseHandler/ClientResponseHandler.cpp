#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t sharedData, va_list args)
		: AExeHandler(sharedData), _request(*va_arg(args, request_t*)) {
		va_end(args);
	}
	ClientResponseHandler::~ClientResponseHandler() {}

	void ClientResponseHandler::handleEvent() {
		if (this->removeHandlerIfNecessary())
			return;
	}

}  // namespace reactor
