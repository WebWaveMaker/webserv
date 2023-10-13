#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include "Server.h"

class ServerConfig;
class Client;
class ServerEventHandler;
class AccessLogger;
class ErrorLogger;

class Server {
   private:
	const ServerConfig& _serverConfig;
	int _fd;
	struct sockaddr_in _serverAddr;
	std::map<int, Client*> _clients;
	ServerEventHandler* _eventHandler;
	AccessLogger* _accessLogger;
	ErrorLogger* _errorLogger;

	Server(const Server& obj);
	Server& operator=(const Server& obj);

   public:
	Server(ServerConfig& serverConfig);
	int getFd() const;
	const ServerConfig& getConfig() const;
	const sockaddr_in& getAddr() const;
	ServerEventHandler& getEventHandler() const;
	AccessLogger& getAccessLogger() const;
	ErrorLogger& getErrorLogger() const;

	~Server();
};

#endif
