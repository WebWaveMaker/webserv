#include "Server.hpp"

/*
	추가 생성필요
	ServerEventHandler* eventHandler;
	AccessLogger* accessLoger;
	ErrorLogger* errorLogger;
*/
Server::Server(ServerConfig& serverConfig) : _serverConfig(serverConfig) {
	std::cout << "server constructor called\n";
	// logger 생성
	// this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0) {
		//errorlog
		throw std::runtime_error("socket faild\n");
	}

	try {
		std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
		this->_serverAddr.sin_family = AF_INET;
		this->_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		// this->_serverAddr.sin_port = htons(_serverConfig->get(PORT).int;);

		if (bind(this->_fd, (struct sockaddr*)&this->_serverAddr, sizeof(this->_serverAddr)) < 0) {
			throw std::runtime_error("bind() error\n");
		}

		if (listen(this->_fd, 5) < 0) {
			throw std::runtime_error("listen() error\n");
		}
		// Read event 등록
	} catch (std::exception& e) {
		//errorlog
		close(this->_fd);
		throw;
	}
}

int Server::getFd() const {
	return (this->_fd);
}

const ServerConfig& Server::getConfig() const {
	return (this->_serverConfig);
}

const sockaddr_in& Server::getAddr() const {
	return (this->_serverAddr);
}

ServerEventHandler& Server::getEventHandler() const {
	return (*this->eventHandler);
}

AccessLogger& Server::getAccessLogger() const {
	return (*this->accessLoger);
}

ErrorLogger& Server::getErrorLogger() const {
	return (*this->errorLogger);
}

Server::~Server() {
	std::cout << "Server destructor called\n";
	// delete
}
