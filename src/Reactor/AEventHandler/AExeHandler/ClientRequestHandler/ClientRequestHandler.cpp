#include "ClientRequestHandler.hpp"
#include "Dispatcher.hpp"

namespace reactor {
	ClientRequestHandler::ClientRequestHandler(sharedData_t& sharedData, va_list args)
		: AExeHandler(sharedData),
		  _request(ServerManager::getInstance()->getServerConfig(sharedData->getFd())),
		  _writeData(new SharedData(this->getHandle(), EVENT_WRITE, std::vector<char>())) {
		Dispatcher::getInstance()->registerIOHandler<ClientReadHandlerFactory>(sharedData);
		va_end(args);
	}

	ClientRequestHandler::~ClientRequestHandler() {}

	RequestParser& ClientRequestHandler::getRequest() {
		return this->_request;
	}

	void ClientRequestHandler::handleEvent() {
		if (this->removeHandlerIfNecessary() || this->getBuffer().empty())
			return;
		if (this->_writeData->getState() == RESOLVE) {
			this->_writeData->clear();
			return;
		}
		std::string content = std::string(this->getBuffer().begin(), this->getBuffer().begin() + this->getReadByte());
		request_t request = this->_request.parse(content);
		this->getBuffer().clear();
		std::cout << "client request handler" << std::endl;
		if (request.get() && !(request->first == LONG_BODY || request->first == LONG_BODY_DONE)) {
			if (request->first == LONG_FIRST)
				request->first = LONG_BODY;
			Dispatcher::getInstance()->registerExeHandler<ClientResponseHandlerFactory>(this->_writeData, &request);
			std::cout << "ClientResponse register " << std::endl;
			return;
		}
	}
}  // namespace reactor
