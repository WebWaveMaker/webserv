#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include "Server.h"

class Server : public ICallback {
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
	void makeScoket();
	void mallocParameter();
	void bindListen();
	void registerReadEvent();
	int makeFd(const char* path, const char* option);

   public:
	Server(utils::shared_ptr<ServerConfig>& serverConfig);

	virtual Client* createClient(int clientFd, struct sockaddr_in& clientAddr);
	virtual void eraseClient(int key);
	virtual ICallback* getCallback();

	int getFd() const;
	const ServerConfig& getConfig() const;
	const sockaddr_in& getAddr() const;
	AccessLogger& getAccessLogger() const;
	ErrorLogger& getErrorLogger() const;

	~Server();
};

#endif
