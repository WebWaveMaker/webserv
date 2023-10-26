#include "Client.hpp"

Client::Client(int clientFd, const sockaddr_in& clientAddr, ServerConfig& serverConfig)
	: _clientFd(clientFd), _clientAddr(clientAddr), _serverConfig(serverConfig) {
	std::cout << utils::itos(clientFd) + " | client constructor called\n";
	this->_clientAddrStr = "any";
	// _clientAddrStr 초기화
	// this->_req = new RequestParser;
	// this->_res = new ResponseParser;
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

bool Client::executeRequest() {
	return true;
}

Client::~Client() {}
