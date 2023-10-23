#pragma once
#ifndef ICALLBACK_HPP
#define ICALLBACK_HPP

#include "ICallback.h"

class ICallback {
   private:
	ICallback& operator=(const ICallback& obj);
	ICallback(const ICallback& obj);
	ICallback();

   public:
	virtual void eraseClient(int key) = 0;
	virtual Client* createClient(int clientFd, struct sockaddr_in& clientAddr) = 0;
	virtual ICallback* getCallback() = 0;
	virtual ~ICallback();
};

#endif