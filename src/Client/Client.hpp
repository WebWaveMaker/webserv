#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Client.h"

class Config;
class Request;
class Response;
class ClientEventHandler;

class Client {
   private:
	int _clientFd;
	struct sockaddr_in _clientAddr;
	Config& _serverConfig;
	ClientEventHandler* _eventHandler;
	Request* _req;
	Response* _res;

	Client(const Client& obj);
	Client& operator=(const Client& obj);

   public:
	Client(int clientFd, const sockaddr_in& clientAddr, Config& serverConfig);
	~Client();
};

#endif
