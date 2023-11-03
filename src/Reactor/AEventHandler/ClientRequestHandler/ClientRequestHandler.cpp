#include "ClientRequestHandler.hpp"

namespace reactor {
	ClientRequestHandler::ClientRequestHandler(sharedData_t sharedData, va_list args)
		: AEventHandler(sharedData),
		  _request(utils::shared_ptr<RequestParser>(
			  new RequestParser((ServerManager::getInstance()->getServerConfig(sharedData.get()->fd))))) {
		Dispatcher::getInstance()->registerIOHandler<ClientReadHandlerFactory>(sharedData, EVFILT_READ);
		va_end(args);
	}

	ClientRequestHandler::~ClientRequestHandler() {}

	request_t ClientRequestHandler::getRequest() const {
		return this->_request;
	}

	void ClientRequestHandler::handleEvent() {
		request_t request = this->_req.get()->parse(this->getBuffer().data());
		this->getBuffer().clear();
		if (request.get())
			Dispatcher::getInstance()->registerExeHandler<ClientResponseHandlerFactory>(sharedData, request);
		// ClientResponseHandler Registe
	}
}  // namespace reactor