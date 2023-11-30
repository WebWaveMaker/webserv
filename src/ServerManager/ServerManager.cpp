#include "ServerManager.hpp"
#include "Dispatcher.hpp"
#include "Server.hpp"

ServerManager::ServerManager() : _configParser(), _servers() {}

void ServerManager::init(const std::string path, char** envp) {
	this->_envp = envp;
	this->_serverConfigs = this->_configParser.parse(path);
	try {
		this->registerTimeoutEvent();
		this->handleSigPipe();
		this->createServer(this->_serverConfigs);
	} catch (std::exception& e) {
		throw;
	}
}

void ServerManager::handleSigPipe() {
	signal(SIGPIPE, SIG_IGN);
};

void ServerManager::createServer(config_t& ServerConfigs) {
	try {
		for (config_t::iterator it = ServerConfigs.begin(); it != ServerConfigs.end(); ++it) {
			Server* server = new Server(*it);
			std::map<int, Server*>::iterator sIt = this->_servers.find(server->getFd());

			if (sIt == this->_servers.end()) {
				this->_servers[server->getFd()] = server;
			} else {
				delete server;
			}
		}
	} catch (std::exception& e) {
		throw;
	}
}

void ServerManager::createClient(int serverFd, int clientFd, struct sockaddr_in& clientAddr) {
	this->getServer(serverFd)->createClient(clientFd, clientAddr);
}

void ServerManager::eraseClient(fd_t fd) {
	for (std::map<int, Server*>::iterator serverIt = this->_servers.begin(); serverIt != this->_servers.end();
		 ++serverIt) {
		if (serverIt->second->hasClient(fd)) {
			serverIt->second->eraseClient(fd);
			break;
		}
	}
}

Server* ServerManager::getServer(int serverFD) const {
	std::map<int, Server*>::const_iterator it = this->_servers.find(serverFD);

	if (it != this->_servers.end()) {
		return (it->second);
	}
	return NULL;
}

config_t ServerManager::getServerConfigs() const {
	return (this->_serverConfigs);
}

utils::shared_ptr<ServerConfig> ServerManager::getServerConfig(const int clientFd) const {
	for (std::map<int, Server*>::const_iterator serverIt = this->_servers.begin(); serverIt != this->_servers.end();
		 ++serverIt) {
		if (serverIt->second->hasClient(clientFd)) {
			return (serverIt->second->getConfig());
		}
	}
	return u::shared_ptr<ServerConfig>();
}

ServerManager::~ServerManager() {
	for (std::map<int, Server*>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
		delete it->second;
	this->_servers.clear();
}

void ServerManager::registerReadEvent(fd_t fd) {
	reactor::Dispatcher::getInstance()->registerExeHandler<reactor::ServerAcceptHandlerFactory>(
		reactor::sharedData_t(new reactor::SharedData(fd, EVENT_READ, std::vector<char>())));
}

void ServerManager::registerTimeoutEvent() {
	reactor::sharedData_t timersharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(4242, EVENT_TIMER, std::vector<char>()));
	timersharedData->setReadByte(this->_serverConfigs[0]->getDirectives(KEEPALIVE_TIMEOUT).asUint() * 500);
	reactor::Dispatcher::getInstance()->registerExeHandler<reactor::TimeoutHandlerFactory>(timersharedData);
}

utils::shared_ptr<std::vector<fd_t> > ServerManager::getClientFds() {
	utils::shared_ptr<std::vector<fd_t> > clientFds = utils::shared_ptr<std::vector<fd_t> >(new std::vector<fd_t>);

	for (std::map<int, Server*>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it) {
		u::shared_ptr<std::vector<fd_t> > serverClients = it->second->getClientFds();
		std::copy(serverClients->begin(), serverClients->end(), std::back_inserter(*(clientFds.get())));
	}
	return (clientFds);
}

char** ServerManager::getEnvp() const {
	return (this->_envp);
}

std::string ServerManager::getClientIp(fd_t fd) {
	for (std::map<int, Server*>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it) {
		if (it->second->hasClient(fd)) {
			return std::string(it->second->getClientIP(fd));
		}
	}
	return std::string("");
}
