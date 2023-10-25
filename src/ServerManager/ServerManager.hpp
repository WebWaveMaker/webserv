#pragma once
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerManager.h"

class ServerManager {
   private:
	std::vector<ServerConfig*>* _serverConfigs;
	std::map<int, Server*> _servers;

	ServerManager(const ServerManager& obj);
	ServerManager& operator=(const ServerManager& obj);

   public:
	ServerManager(std::vector<ServerConfig*>* serverConfigs);
	void CreateServer(std::vector<ServerConfig*>& serverConfigs);
	Server* getServer(int serverFd) const;
	std::vector<ServerConfig*>* getServerConfigs() const;
	~ServerManager();
};

#endif
