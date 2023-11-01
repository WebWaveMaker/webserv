#include "ServerAcceptHandler.hpp"

reactor::ServerAcceptHandler::ServerAcceptHandler(handle_t fd,
												  u::shared_ptr<std::map<int, u::shared_ptr<Client> > >& clients,
												  u::shared_ptr<AccessLogger>& accessLogger,
												  u::shared_ptr<ErrorLogger>& errorLogger)
	: AEventHandler(fd, accessLogger, errorLogger), _clients(clients) {
	std::cout << "ServerAcceptHandler constructor called\n";
}

handle_t reactor::ServerAcceptHandler::getHandle() const {
	return (this->_fd);
}

void reactor::ServerAcceptHandler::handleEvent() {

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
	reactor::Dispatcher::getInstance()->registerHandler(
		u::static_pointer_cast<reactor::AEventHandler, reactor::ClientReadHandler>(
			u::shared_ptr<reactor::ClientReadHandler>(new reactor::ClientReadHandler(
				clientFd, this->_accessLogger, this->_errorLogger, (*this->_clients)[clientFd]))),
		EVENT_READ);
	TestAcceptHandler::printClientInfo((*this->_clients)[clientFd].get());
}

reactor::ServerAcceptHandler::~ServerAcceptHandler() {
	std::cout << "ServerAcceptHandler Destructor called\n";
}
