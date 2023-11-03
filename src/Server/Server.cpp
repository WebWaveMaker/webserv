#include "Server.hpp"

Server::Server(utils::shared_ptr<ServerConfig>& serverConfig) : _serverConfig(serverConfig) {
	std::cout << "server constructor called\n";
	try {
		this->listenServer();
	} catch (std::exception& e) {
		throw;
	}
}

void Server::listenServer() {

	this->makeScoket();

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

	reactor::Dispatcher* dispatcher = reactor::Dispatcher::getInstance();

	dispatcher->registerHandler(
		u::shared_ptr<reactor::AEventHandler>(static_cast<reactor::AEventHandler*>(
			new reactor::ServerAcceptHandler(this->_fd, this->_clients, this->_accessLogger, this->_errorLogger))),
		EVENT_READ);
}

void Server::bindListen() {

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
}

void Server::makeScoket() {

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (this->_fd < 0) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw;
	}

	int opt = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		throw std::runtime_error("setsockopt() failed\n");
	}
}

void Server::mallocParameter() {

	std::pair<std::string, LogLevels> errorLog = this->_serverConfig.get()->getDirectives(ERROR_LOG).asLog();
	const int errorFd = this->makeFd(errorLog.first.c_str(), "w");

	this->_clients =
		utils::shared_ptr<std::map<int, utils::shared_ptr<Client> > >(new std::map<int, utils::shared_ptr<Client> >);
	this->_accessLogger = utils::shared_ptr<AccessLogger>(new AccessLogger(STDOUT_FILENO));
	this->_errorLogger = utils::shared_ptr<ErrorLogger>(new ErrorLogger(errorFd, errorLog.second));
}

int Server::makeFd(const char* path, const char* option) {
	FILE* file = fopen(path, option);
	if (file == NULL) {
		this->_errorLogger->systemCallError(__FILE__, __LINE__, __func__);
		throw;
	}
	const int fileFd = fileno(file);
	return (fileFd);
}

Client* Server::createClient(int clientFd, struct sockaddr_in& clientAddr) {
	try {
		Client* newClient = new Client(clientFd, clientAddr, this->_serverConfig,
									   utils::shared_ptr<RequestParser>(new RequestParser(this->_serverConfig)));
		return (newClient);
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

Server::~Server() {
	std::cout << "Server destructor called\n";

	this->_clients->clear();
}
