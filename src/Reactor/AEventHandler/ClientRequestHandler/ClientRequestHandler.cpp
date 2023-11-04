#include "ClientRequestHandler.hpp"

namespace reactor {
	ClientRequestHandler::ClientRequestHandler(sharedData_t sharedData, va_list args)
		: AEventHandler(sharedData), _request(ServerManager::getInstance()->getServerConfig(sharedData.get()->fd)) {
		Dispatcher::getInstance()->registerIOHandler<ClientReadHandlerFactory>(sharedData);
		va_end(args);
	}

	ClientRequestHandler::~ClientRequestHandler() {}

	RequestParser& ClientRequestHandler::getRequest() {
		return this->_request;
	}

	void ClientRequestHandler::handleEvent() {
		request_t request = this->_request.parse(this->getBuffer().data());
		this->getBuffer().clear();
		if (request.get())
			Dispatcher::getInstance()->registerExeHandler<ClientResponseHandlerFactory>(this->_sharedData, &request);
	}
}  // namespace reactor