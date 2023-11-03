#include "ServerAcceptHandler.hpp"

namespace reactor {

	ServerAcceptHandler::ServerAcceptHandler(sharedData_t sharedData) : AEventHandler(sharedData) {
		std::cout << "ServerAcceptHandler constructor called\n";
	}

	void ServerAcceptHandler::handleEvent() {
		if (this->getState() != ACCEPT)
			return;
		sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		int clientFd = accept(this->_fd, (struct sockaddr*)&clientAddr, &clientAddrLen);
		if (clientFd < 0) {
			_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
			throw;
		}

		std::map<int, u::shared_ptr<Client> >::iterator it = this->_clients->find(clientFd);

		try {
			if (it == this->_clients->end()) {
				(*this->_clients)[clientFd] =
					u::shared_ptr<Client>(ServerManager::getInstance()->createClient(this->_fd, clientFd, clientAddr));
				this->_accessLogger->log(u::itos(clientFd) + "client access to Server", __func__, GET,
										 (*this->_clients)[clientFd].get());
			} else
				close(clientFd);
		} catch (std::exception& e) {
			this->_errorLogger->log("error in accept", __func__, LOG_ERROR, 0);
			throw;
		}
		if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
			this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
			throw;
		}
		Dispatcher::getInstance()->registerExeHandler<ClientRequestHandlerFactory>(
			u::shared_ptr<sharedData_t>(new sharedData_t(clientFd, std::vector<char>())));
		TestAcceptHandler::printClientInfo((*this->_clients)[clientFd].get());
	}

	ServerAcceptHandler::~ServerAcceptHandler() {
		std::cout << "ServerAcceptHandler Destructor called\n";
	}

}  // namespace reactor