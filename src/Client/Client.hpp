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
	utils::shared_ptr<RequestParser> _req;
	ResponseParser* _res;
	std::size_t _cnt;  // 0,1 이아니면 해당 클라이언트의 이벤트가 등록되어있는 것임. fd close를 하지 않기 위함.

	Client(const Client& obj);
	Client& operator=(const Client& obj);

   public:
	Client(int clientFd, const sockaddr_in& clientAddr, utils::shared_ptr<ServerConfig>& serverConfig,
		   utils::shared_ptr<RequestParser> req);
	bool executeRequest();

	int getFd() const;
	const std::string& getAddrStr() const;
	struct sockaddr_in getAddr() const;
	ServerConfig& getConfig() const;
	utils::shared_ptr<RequestParser> getReqParser() const;
	ResponseParser* getResParser() const;

	~Client();
};

#endif
