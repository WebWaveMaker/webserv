#include "Client.hpp"

Client::Client(int clientFd, const sockaddr_in& clientAddr) : _clientFd(clientFd), _clientAddr(clientAddr) {}

int Client::getFd() const {
	return (this->_clientFd);
}

struct sockaddr_in Client::getAddr() const {
	return (this->_clientAddr);
}

Client::~Client() {
	close(this->_clientFd);
	std::cerr << "Client " << this->_clientFd << " deleted" << std::endl;
}
