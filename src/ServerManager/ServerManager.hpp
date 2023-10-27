#pragma once
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerManager.h"

class ServerManager {
   private:
	std::vector<u::shared_ptr<ServerConfig> > _serverConfigs;
	std::map<int, Server*> _servers;

	ServerManager(const ServerManager& obj);
	ServerManager& operator=(const ServerManager& obj);

   public:
	ServerManager(std::vector<utils::shared_ptr<ServerConfig> >& serverConfigs);
	void CreateServer(std::vector<u::shared_ptr<ServerConfig> >& serverConfigs);
	Server* getServer(int serverFd) const;
	std::vector<u::shared_ptr<ServerConfig> > getServerConfigs() const;
	~ServerManager();
};

#endif
