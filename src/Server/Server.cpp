#include "Server.hpp"

Server::Server(ServerConfig& serverConfig) : _serverConfig(serverConfig) {
	std::cout << "server constructor called\n";

	this->_clients = new std::map<int, ClientEventHandler*>;

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw std::runtime_error("socket faild\n");
	}
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
		this->_eventHandler =
			new ServerEventHandler(this->_fd, this, this->_clients, this->_accessLogger, this->_errorLogger);
		reactor::Dispatcher::getInstance()->registerHander(this->_eventHandler, EVENT_READ);
	} catch (std::exception& e) {
		close(this->_fd);
		throw;
	}
}

void Server::registerEvent(EventType type) {
	if (type == EVENT_READ) {
		reactor::Dispatcher* dispatcher = reactor::Dispatcher::getInstance();
		dispatcher->registerHander(this->_eventHandler, EVENT_READ);
	}
}

void Server::eraseClient(int key) {
	this->removeClient(key);
}

Client* Server::createClient(int clientFd, struct sockaddr_in& clientAddr) {
	try {
		Client* newClient = new Client(clientFd, clientAddr, this->_serverConfig);
		return (newClient);
	} catch (std::exception& e) {
		throw;
	}
}

ICallback* Server::getCallback() {
	return (this);
}

void Server::removeClient(int key) {
	std::map<int, ClientEventHandler*>::iterator it = this->_clients->find(key);

	if (it != this->_clients->end()) {
		delete it->second;
		this->_clients->erase(key);
	} else {
		this->_errorLogger->log("Not Found client\n", __func__, LOG_ERROR, NULL);
		throw std::runtime_error("removeClient Error\n");
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

	for (std::map<int, ClientEventHandler*>::iterator it = this->_clients->begin(); it != this->_clients->end(); ++it)
		delete it->second;
	this->_clients->clear();
	// removeHandler() 고려
	delete _eventHandler;
	delete this->_accessLogger;
	delete this->_errorLogger;
}
