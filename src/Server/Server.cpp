#include "Server.hpp"

Server::Server(ServerConfig& serverConfig) : _serverConfig(serverConfig) {
	std::cout << "server constructor called\n";

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw std::runtime_error("socket faild\n");
	}

	// this->_eventHandler = ServerEventHandler();
	this->_accessLogger = new AccessLogger(this->_fd);
	this->_errorLogger = new ErrorLogger(this->_fd, LOG_ERROR);

	try {
		std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
		this->_serverAddr.sin_family = AF_INET;
		this->_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		this->_serverAddr.sin_port = htons(_serverConfig.getDirectives(LISTEN).asUint());

		if (bind(this->_fd, (struct sockaddr*)&this->_serverAddr, sizeof(this->_serverAddr)) < 0) {
			this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
			throw std::runtime_error("bind() error\n");
		}

		if (listen(this->_fd, 5) < 0) {
			this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
			throw std::runtime_error("listen() error\n");
		}

		// ReadEvent 등록
		// this->registerEvent(READ);
	} catch (std::exception& e) {
		close(this->_fd);
		throw;
	}
}

// void Server::registerEvent(EventType type) {
// }

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
	return (*(this->_eventHandler));
}

AccessLogger& Server::getAccessLogger() const {
	return (*(this->_accessLogger));
}

ErrorLogger& Server::getErrorLogger() const {
	return (*(this->_errorLogger));
}

Server::~Server() {
	std::cout << "Server destructor called\n";
	// delete
	for (std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		delete it->second;
	this->_clients.clear();
	// delete _eventHandler;
	delete this->_accessLogger;
	delete this->_errorLogger;
}
