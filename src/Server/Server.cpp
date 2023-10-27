#include "Server.hpp"

Server::Server(utils::shared_ptr<ServerConfig>& serverConfig) : _serverConfig(serverConfig) {
	std::cout << "server constructor called\n";
	this->listenServer();
}

void Server::listenServer() {
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw std::runtime_error("socket faild\n");
	}

	this->_clients =
		utils::shared_ptr<std::map<int, utils::shared_ptr<Client> > >(new std::map<int, utils::shared_ptr<Client> >);
	this->_accessLogger = utils::shared_ptr<AccessLogger>(new AccessLogger(this->_fd));
	this->_errorLogger = utils::shared_ptr<ErrorLogger>(new ErrorLogger(this->_fd, LOG_ERROR));

	try {
		std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
		this->_serverAddr.sin_family = AF_INET;
		this->_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		this->_serverAddr.sin_port = htons(this->_serverConfig.get()->getDirectives(LISTEN).asUint());

		if (bind(this->_fd, (struct sockaddr*)&this->_serverAddr, sizeof(this->_serverAddr)) < 0) {
			this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
			throw std::runtime_error("bind() error\n");
		}

		if (listen(this->_fd, 5) < 0) {
			this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
			throw std::runtime_error("listen() error\n");
		}

		// ReadEvent 등록
	} catch (std::exception& e) {
		close(this->_fd);
		throw;
	}
}

Client* Server::createClient(int clientFd, struct sockaddr_in& clientAddr) {
	try {
		Client* newClient = new Client(clientFd, clientAddr, this->_serverConfig);
		return (newClient);
	} catch (std::exception& e) {
		throw;
	}
}

void Server::eraseClient(int key) {
	this->removeClient(key);
}

ICallback* Server::getCallback() {
	return (this);
}

void Server::removeClient(int key) {
	std::map<int, utils::shared_ptr<Client> >::iterator it = this->_clients.get()->find(key);

	if (it != this->_clients->end()) {
		it->second.release();
		this->_clients.get()->erase(key);
	} else {
		this->_errorLogger->log("Not Found client\n", __func__, LOG_ERROR, NULL);
		throw std::runtime_error("removeClient Error\n");
	}
}

int Server::getFd() const {
	return (this->_fd);
}

const ServerConfig& Server::getConfig() const {
	return (*(this->_serverConfig.get()));
}

const sockaddr_in& Server::getAddr() const {
	return (this->_serverAddr);
}

AccessLogger& Server::getAccessLogger() const {
	return (*(this->_accessLogger));
}

ErrorLogger& Server::getErrorLogger() const {
	return (*(this->_errorLogger));
}

Server::~Server() {
	std::cout << "Server destructor called\n";

	for (std::map<int, utils::shared_ptr<Client> >::iterator it = this->_clients->begin(); it != this->_clients->end();
		 ++it)
		it->second.release();
	this->_clients.get()->clear();
}
