#pragma once
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerManager.h"

class ConfigStore;

class ServerManager {
   private:
	ConfigStore& _config;
	std::map<int, Server*> _servers;

	ServerManager(const ServerManager& obj);
	ServerManager& operator=(const ServerManager& obj);

   public:
	ServerManager(ConfigStore& configStore);
	void CreateServer(std::vector<ServerConfig*>& serverConfigs);
	Server* getServer(int serverFd) const;
	const ConfigStore& getConfigStore() const;
	~ServerManager();
};

#endif
