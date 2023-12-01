#include "ClientRequestHandler.hpp"
#include "Dispatcher.hpp"

namespace reactor {
	ClientRequestHandler::ClientRequestHandler(sharedData_t& sharedData)
		: AExeHandler(sharedData),
		  _requestParser(sharedData->getFd(),
						 ServerManager::getInstance()->getServerDefaultConfig(sharedData->getFd())),
		  _writeData(new SharedData(this->getHandle(), EVENT_WRITE, std::vector<char>())) {
		Dispatcher::getInstance()->registerIOHandler<ClientReadHandlerFactory>(sharedData);
	}

	ClientRequestHandler::~ClientRequestHandler() {}

	RequestParser& ClientRequestHandler::getRequest() {
		return this->_requestParser;
	}

	void ClientRequestHandler::handleEvent() {
		if (this->removeHandlerIfNecessary() || this->getBuffer().empty())
			return;
		if (this->_writeData->getState() == RESOLVE) {
			this->_writeData->clear();
			return;
		}
		std::string content = std::string(this->getBuffer().begin(), this->getBuffer().begin() + this->getReadByte());
		request_t request = this->_requestParser.parse(content);
		this->getBuffer().clear();
		if (request.get() && request->second.getIsRegistered() == false) {
			request->second.setIsRegistered(true);
			this->_writeData->setRequest(request);
			Dispatcher::getInstance()->registerExeHandler<ClientResponseHandlerFactory>(this->_writeData);
		}
	}
}  // namespace reactor
