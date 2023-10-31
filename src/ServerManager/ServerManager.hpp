#pragma once
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerManager.h"

class ServerManager : public u::TSingleton<ServerManager> {
   private:
	ConfigParser _configParser;
	config_t _serverConfigs;
	std::map<int, Server*> _servers;

	ServerManager(const ServerManager& obj);
	ServerManager& operator=(const ServerManager& obj);

   public:
	ServerManager();
	ServerManager(const std::string path);
	void init(const std::string path);
	void CreateServer(config_t& serverConfigs);
	Server* getServer(int serverFd) const;
	config_t getServerConfigs() const;
	~ServerManager();
};

#endif
