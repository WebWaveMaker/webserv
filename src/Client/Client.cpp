#include "Client.hpp"

Client::Client(int clientFd, const sockaddr_in& clientAddr, Config& serverConfig)
	: _clientFd(clientFd), _clientAddr(clientAddr), _serverConfig(serverConfig) {
	std::cout << clientFd + " | client constructor called\n";

	// request 생성
	// response 생성
	// EventHandler 생성
}
