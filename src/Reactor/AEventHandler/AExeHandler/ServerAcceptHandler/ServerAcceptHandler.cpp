#include "ServerAcceptHandler.hpp"

namespace reactor {

	ServerAcceptHandler::ServerAcceptHandler(sharedData_t sharedData) : AExeHandler(sharedData) {
		Dispatcher::getInstance()->registerIOHandler<ServerReadHandlerFactory>(sharedData);
		std::cout << "ServerAcceptHandler constructor called\n";
	}

	void ServerAcceptHandler::handleEvent() {
		if (this->getState() != ACCEPT)
			return;
		sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		int clientFd = accept(this->getHandle(), (struct sockaddr*)&clientAddr, &clientAddrLen);
		if (clientFd < 0) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
			throw;
		}

		try {
			ServerManager::getInstance()->createClient(this->getHandle(), clientFd, clientAddr);
		} catch (std::exception& e) {
			close(clientFd);
			// this->_errorLogger->log("error in accept", __func__, LOG_ERROR, 0);
			throw;
		}
		if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
			// this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
			throw;
		}
		Dispatcher::getInstance()->registerExeHandler<ClientRequestHandlerFactory>(
			sharedData_t(new sharedData(clientFd, EVENT_READ, std::vector<char>())));
		this->setState(PENDING);
		// Dispatcher::getInstance()->removeExeHandler(u::shared_ptr<AEventHandler>(this));
	}

	ServerAcceptHandler::~ServerAcceptHandler() {
		std::cout << "ServerAcceptHandler Destructor called\n";
	}

}  // namespace reactor
