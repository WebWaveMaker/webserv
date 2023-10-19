#include "ServerEventHandler.hpp"

// ServerEventHandler(std::map<int, Client*>* clients);
// 	virtual handle_t getHandle() const;
// 	virtual void handleRead();
// 	virtual void handleWrite();
// 	virtual void handleError();
// 	virtual ~ServerEventHandler();

ServerEventHandler::ServerEventHandler(handle_t handleFd, std::map<int, Client*>* clients)
	: _handleFd(handleFd), _clients(clients) {
	std::cout << this->_handleFd << " | ServerEventHandler constructor called\n";
	// AccessLogger* _accessLogger;
	// ErrorLogger* _errorLogger;
	//
}

handle_t ServerEventHandler::getHandle() const {
	return (this->_handleFd);
}

void ServerEventHandler::handleRead() {
	std::cout << "Server handleRead called\n";

	sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int clientFd = accept(this->_handleFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
}
