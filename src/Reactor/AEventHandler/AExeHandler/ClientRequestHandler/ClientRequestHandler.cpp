#include "ClientRequestHandler.hpp"
#include "Dispatcher.hpp"

namespace reactor {
	ClientRequestHandler::ClientRequestHandler(sharedData_t& sharedData, va_list args)
		: AExeHandler(sharedData),
		  _request(ServerManager::getInstance()->getServerConfig(sharedData.get()->getFd())),
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
		if (this->_writeData.get()->getState() == RESOLVE) {
			this->setState(TERMINATE);
			return;
		}
		request_t request = this->_request.parse(this->getBuffer().data());
		this->getBuffer().clear();
		std::cout << "this?" << std::endl;
		if (request.get()) {
			Dispatcher::getInstance()->registerExeHandler<ClientResponseHandlerFactory>(this->_writeData, &request);
		}
	}
}  // namespace reactor
