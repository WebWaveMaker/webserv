#include "Client.hpp"

Client::Client(int clientFd, const sockaddr_in& clientAddr, ServerConfig& serverConfig)
	: _clientFd(clientFd), _clientAddr(clientAddr), _serverConfig(serverConfig) {
	std::cout << clientFd + " | client constructor called\n";

	// _clientAddrStr 초기화
	// request 생성
	// response 생성
}

int Client::getFd() const {
	return (this->_clientFd);
}

const std::string& Client::getAddrStr() const {
	return (this->_clientAddrStr);
}

struct sockaddr_in Client::getAddr() const {
	return (this->_clientAddr);
}

ServerConfig& Client::getConfig() const {
	return (this->_serverConfig);
}

RequestParser* Client::getReqParser() const {
	return (this->_req);
}

ResponseParser* Client::getResParser() const {
	return (this->_res);
}

bool Client::executeRequest() {}