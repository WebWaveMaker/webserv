#include "ServerAcceptHandler.hpp"
#include "AExeHandler.hpp"
#include "Dispatcher.hpp"

namespace reactor {

	ServerAcceptHandler::ServerAcceptHandler(sharedData_t& sharedData) : AExeHandler(sharedData) {
		Dispatcher::getInstance()->registerIOHandler<ServerReadHandlerFactory>(sharedData);
	}

	void ServerAcceptHandler::handleEvent() {
		if (this->getState() != ACCEPT || this->removeHandlerIfNecessary())
			return;
		sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		int clientFd = accept(this->getHandle(), (struct sockaddr*)&clientAddr, &clientAddrLen);
		if (clientFd < 0) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
			return;
		}

		try {
			ServerManager::getInstance()->createClient(this->getHandle(), clientFd, clientAddr);
		} catch (std::exception& e) {
			close(clientFd);
			return;
		}
		if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
			std::cout << clientFd << std::endl;
			ServerManager::getInstance()->eraseClient(clientFd);
			close(clientFd);
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
			return;
		}
		Dispatcher::getInstance()->registerExeHandler<ClientRequestHandlerFactory>(
			sharedData_t(new SharedData(clientFd, EVENT_READ, std::vector<char>())));
		SyncEventDemultiplexer::getInstance()->setFdTime(clientFd, std::time(NULL));
		this->setState(PENDING);
		// Dispatcher::getInstance()->removeExeHandler(u::shared_ptr<AEventHandler>(this));
	}

	ServerAcceptHandler::~ServerAcceptHandler() {}

}  // namespace reactor
