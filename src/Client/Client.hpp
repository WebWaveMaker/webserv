#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Client.h"
class Client {
   private:
	int _clientFd;
	std::string _clientAddrStr;
	struct sockaddr_in _clientAddr;	 // 127.0.0.1
	utils::shared_ptr<ServerConfig> _serverConfig;
	RequestParser* _req;
	ResponseParser* _res;

	Client(const Client& obj);
	Client& operator=(const Client& obj);

   public:
	Client(int clientFd, const sockaddr_in& clientAddr, utils::shared_ptr<ServerConfig>& serverConfig);
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
