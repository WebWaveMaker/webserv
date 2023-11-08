#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Client.h"

class Client {
   private:
	int _clientFd;
	struct sockaddr_in _clientAddr;	 // 127.0.0.1

	Client(const Client& obj);
	Client& operator=(const Client& obj);

   public:
	Client(int clientFd, const sockaddr_in& clientAddr);

	int getFd() const;
	struct sockaddr_in getAddr() const;
	~Client();
};

#endif
