#include "Client.hpp"

Client::Client(int clientFd, const sockaddr_in& clientAddr, utils::shared_ptr<ServerConfig>& serverConfig,
			   utils::shared_ptr<RequestParser> req)
	: _clientFd(clientFd), _clientAddr(clientAddr), _serverConfig(serverConfig), _req(req) {
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
	return (*(this->_serverConfig.get()));
}

utils::shared_ptr<RequestParser> Client::getReqParser() const {
	return (this->_req);
}

ResponseParser* Client::getResParser() const {
	return (this->_res);
}

bool Client::executeRequest() {
	return true;
}

Client::~Client() {
	close(this->_clientFd);
}
