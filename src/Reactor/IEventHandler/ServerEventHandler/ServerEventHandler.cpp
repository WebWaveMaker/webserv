#include "ServerEventHandler.hpp"

// ServerEventHandler(std::map<int, Client*>* clients);
// 	virtual handle_t getHandle() const;
// 	virtual void handleRead();
// 	virtual void handleWrite();
// 	virtual void handleError();
// 	virtual ~ServerEventHandler();

ServerEventHandler::ServerEventHandler(handle_t handleFd, std::map<int, Client*>* clients, AccessLogger* accessLogger,
									   ErrorLogger* errorLogger)
	: _handleFd(handleFd), _clients(clients), _accessLogger(accessLogger), _errorLogger(errorLogger) {
	std::cout << this->_handleFd << " | ServerEventHandler constructor called\n";
}

handle_t ServerEventHandler::getHandle() const {
	return (this->_handleFd);
}

void ServerEventHandler::handleRead() {
	std::cout << "Server handleRead called\n";

	sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int clientFd = accept(this->_handleFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	if (clientFd < 0) {
		_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw;
	}

	std::map<int, Client*>::iterator it = _clients->find(clientFd);
	try {
		if (it == _clients->end()) {
			// _clients[clientFd] = new Client(clientFd, clientAddr);
			_accessLogger->log("client access to Server", __func__, GET, (*_clients)[clientFd]);
		} else
			close(clientFd);
	} catch (std::exception& e) {
		_errorLogger->log("error in accept", __func__, LOG_ERROR, 0);
		throw;
	}
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	// client의 read 이벤트 등록
}

void ServerEventHandler::handleWrite() {}
void ServerEventHandler::handleError() {}

ServerEventHandler::~ServerEventHandler() {
	std::cout << "ServerEventHandler destructor called\n";
}
