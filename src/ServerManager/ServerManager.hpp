#pragma once
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerManager.h"

class Server;

class ServerManager : public u::TSingleton<ServerManager> {
   private:
	ConfigParser _configParser;
	config_t _serverConfigs;
	std::map<int, Server*> _servers;
	char** _envp;

   public:
	ServerManager();
	void init(int ac, char**&, char**& envp);
	void handleSigPipe();
	void createServer(config_t& serverConfigs);
	void createClient(int serverFd, int clientFd, struct sockaddr_in& clientAddr);
	void eraseClient(fd_t fd);
	Server* getServer(int serverFd) const;
	utils::shared_ptr<ServerConfig> getServerConfig(const int clientFd) const;
	utils::shared_ptr<ServerConfig> getServerConfig(const int clientFd, request_t request) const;
	config_t getServerConfigs() const;
	utils::shared_ptr<std::vector<fd_t> > getClientFds();
	void registerReadEvent(fd_t fd);
	void registerTimeoutEvent();
	std::string getClientIp(fd_t fd);
	char** getEnvp() const;
	~ServerManager();
};

#endif
