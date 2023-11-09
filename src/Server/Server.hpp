#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include "Server.h"

class Server {
   private:
	utils::shared_ptr<ServerConfig> _serverConfig;
	int _fd;
	struct sockaddr_in _serverAddr;
	utils::shared_ptr<std::map<int, utils::shared_ptr<Client> > > _clients;
	utils::shared_ptr<AccessLogger> _accessLogger;
	utils::shared_ptr<ErrorLogger> _errorLogger;

	Server(const Server& obj);
	Server& operator=(const Server& obj);

	void removeClient(int key);
	void listenServer();
	void makeSocket();
	void mallocParameter();
	void bindListen();
	void registerReadEvent();
	int makeFd(const char* path, const char* option);

   public:
	Server(utils::shared_ptr<ServerConfig>& serverConfig);

	void createClient(int clientFd, struct sockaddr_in& clientAddr);
	void eraseClient(int key);

	bool hasClient(int key);
	int getFd() const;
	utils::shared_ptr<ServerConfig> getConfig() const;
	const sockaddr_in& getAddr() const;
	AccessLogger& getAccessLogger() const;
	ErrorLogger& getErrorLogger() const;
	utils::shared_ptr<std::vector<fd_t> > getClientFds();

	~Server();
};

#endif
