#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Client.h"

class ServerConfig;
class RequestParser;
class ResponseParser;
class ClientEventHandler;

class Client {
   private:
	int _clientFd;
	const std::string _clientAddrStr;
	struct sockaddr_in _clientAddr;
	ServerConfig& _serverConfig;
	RequestParser* _req;
	ResponseParser* _res;

	Client(const Client& obj);
	Client& operator=(const Client& obj);

   public:
	Client(int clientFd, const sockaddr_in& clientAddr, ServerConfig& serverConfig);
	bool executeRequest();

	int getFd() const;
	const std::string& getAddrStr() const;
	struct sockaddr_in getAddr() const;
	ServerConfig& getConfig() const;
	RequestParser* getReqParser() const;
	ResponseParser* getResParser() const;

	~Client();
};

#endif
