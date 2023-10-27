#include "ServerAcceptHandler.hpp"

ServerAcceptHandler::ServerAcceptHandler(handle_t fd, ICallback* callback,
										 u::shared_ptr<std::map<int, u::shared_ptr<Client> > >& clients,
										 u::shared_ptr<AccessLogger>& accessLogger,
										 u::shared_ptr<ErrorLogger>& errorLogger)
	: AEventHandler(fd, accessLogger, errorLogger), _callback(callback), _clients(clients) {
	std::cout << "ServerAcceptHandler constructor called\n";
}

handle_t ServerAcceptHandler::getHandle() const {
	return (this->_fd);
}

void ServerAcceptHandler::handleEvent() {

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
			(*this->_clients.get())[clientFd] =
				u::shared_ptr<Client>(this->_callback->createClient(clientFd, clientAddr));
			this->_accessLogger->log(u::itos(clientFd) + "client access to Server", __func__, GET, u::nullptr_t);
		} else
			close(clientFd);
	} catch (std::exception& e) {
		this->_errorLogger->log("error in accept", __func__, LOG_ERROR, 0);
		throw;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw std::runtime_error("");
	}
	// reactor::Dispatcher* dispatcher = reactor::Dispatcher::getInstance();
	// dispatcher->registerHander((*this->_clients)[clientFd], EVENT_READ);
}

ServerAcceptHandler::~ServerAcceptHandler() {
	std::cout << "ServerAcceptHandler Destructor called\n";
}
