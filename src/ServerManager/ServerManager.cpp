#include "ServerManager.hpp"

ServerManager::ServerManager() : _configParser(), _servers() {}

void ServerManager::init(const std::string path) {
	this->_serverConfigs = this->_configParser.parse(path);
	try {
		this->CreateServer(this->_serverConfigs);
	} catch (std::exception& e) {
		throw;
	}
}

void ServerManager::CreateServer(config_t& ServerConfigs) {
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

ServerManager::~ServerManager() {
	std::cout << "ServerManager Destructor called\n";

	for (std::map<int, Server*>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
		delete it->second;
	this->_servers.clear();
}
