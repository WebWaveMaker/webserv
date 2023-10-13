#include "ServerManager.hpp"

// class ServerManager {
//    private:
// 	ConfigStore& _config;
// 	std::map<int, Server&> _servers;

// 	ServerManager(const ServerManager& obj);
// 	ServerManager& operator=(const ServerManager& obj);

//    public:
// 	ServerManager(ConfigStore& configStore);
// 	void CreateServer(std::vector<ServerConfig&> serverConfigs);
// 	Server* getServer(int serverFd) const;
// 	const ConfigStore& getConfigStore() const;
// 	~ServerManager();
// };

ServerManager::ServerManager(ConfigStore& configStore) : _config(configStore) {
	try {
		// this->CreateServer(configStore.getServerConfig(););
	} catch (std::exception& e) {
		throw;
	}
}

void ServerManager::CreateServer(std::vector<ServerConfig*>& ServerConfigs) {
	try {
		for (std::vector<ServerConfig&>::iterator it = ServerConfigs.begin(); it != ServerConfigs.end(); ++it) {
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

Server* ServerManager::getServer(int serverFD) const {
	std::map<int, Server*>::const_iterator it = this->_servers.find(serverFD);

	if (it != this->_servers.end()) {
		return (it->second);
	}
	return NULL;
}

ServerManager::~ServerManager() {
	std::cout << "ServerManager Destructor called\n";

	for (std::map<int, Server*>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
		delete it->second;
	this->_servers.clear();

	delete &this->_config;	// 힙영역에 할당된 경우 사용
}
