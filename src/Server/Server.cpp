#include "Server.hpp"

Server::Server(utils::shared_ptr<ServerConfig>& serverConfig) : _serverConfig(serverConfig) {
	try {
		this->listenServer();
	} catch (std::exception& e) {
		throw;
	}
}

Server::Server(const Server& obj) {
	*this = obj;
}

Server& Server::operator=(const Server& obj) {
	this->_serverConfig = obj._serverConfig;
	this->_fd = obj._fd;
	this->_serverAddr = obj._serverAddr;
	this->_clients = obj._clients;
	this->_accessLogger = obj._accessLogger;
	this->_errorLogger = obj._errorLogger;
	return (*this);
}

void Server::listenServer() {

	this->makeSocket();

	try {
		this->mallocParameter();
		this->bindListen();
		this->registerReadEvent();
	} catch (std::exception& e) {
		close(this->_fd);
		throw;
	}
}

void Server::registerReadEvent() {
	ServerManager::getInstance()->registerReadEvent(this->_fd);
}

void Server::bindListen() {

	std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_serverAddr.sin_port = htons(this->_serverConfig->getDirectives(LISTEN).asUint());

	if (bind(this->_fd, (struct sockaddr*)&this->_serverAddr, sizeof(this->_serverAddr)) < 0) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw;
	}

	if (listen(this->_fd, 128) < 0) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw;
	}
}

void Server::makeSocket() {

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (this->_fd < 0) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw;
	}
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		throw;
	}

	int opt = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		throw;
	}
}

void Server::mallocParameter() {

	std::pair<std::string, LogLevels> errorLog = this->_serverConfig->getDirectives(ERROR_LOG).asLog();
	const int errorFd = utils::makeFd(("." + errorLog.first).c_str(), "w");

	this->_clients =
		utils::shared_ptr<std::map<int, utils::shared_ptr<Client> > >(new std::map<int, utils::shared_ptr<Client> >);
	this->_accessLogger = utils::shared_ptr<AccessLogger>(new AccessLogger(STDOUT_FILENO));
	this->_errorLogger = utils::shared_ptr<ErrorLogger>(new ErrorLogger(errorFd, errorLog.second));
}

void Server::createClient(int clientFd, struct sockaddr_in& clientAddr) {
	try {
		utils::shared_ptr<Client> newClient(new Client(clientFd, clientAddr));
		(*this->_clients.get())[clientFd] = newClient;
		this->_accessLogger->log(this->_serverConfig->getDirectives(SERVER_NAME).asString(), __func__, UNKNOWN,
								 newClient.get());
	} catch (std::exception& e) {
		throw;
	}
}

bool Server::hasClient(int key) {
	return (this->_clients->find(key) != this->_clients->end());
}

void Server::eraseClient(int key) {
	this->removeClient(key);
}

void Server::removeClient(int key) {
	std::map<int, utils::shared_ptr<Client> >::iterator it = this->_clients->find(key);

	if (it != this->_clients->end()) {
		this->_accessLogger->log(this->_serverConfig->getDirectives(SERVER_NAME).asString(), __func__, UNKNOWN,
								 it->second.get());
		this->_clients->erase(it);
	} else {
		this->_errorLogger->log("Not Found client\n", __func__, LOG_ERROR, NULL);
	}
}

int Server::getFd() const {
	return (this->_fd);
}

utils::shared_ptr<ServerConfig> Server::getConfig() const {
	return this->_serverConfig;
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

utils::shared_ptr<std::vector<fd_t> > Server::getClientFds() {
	utils::shared_ptr<std::vector<fd_t> > clientFds = utils::shared_ptr<std::vector<fd_t> >(new std::vector<fd_t>);

	for (std::map<int, u::shared_ptr<Client> >::iterator it = this->_clients->begin(); it != this->_clients->end();
		 ++it) {
		clientFds->push_back(it->first);
	}
	return (clientFds);
}

std::string Server::getClientIP(fd_t fd) {
	if (this->hasClient(fd)) {
		char* clientIP = inet_ntoa((*this->_clients.get())[fd].get()->getAddr().sin_addr);
		return std::string(clientIP);
	}
	return "";
}

Server::~Server() {
	this->_clients->clear();
}
