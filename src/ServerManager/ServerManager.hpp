#pragma once
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerManager.h"

class ServerManager {
   private:
   public:
	ServerManager();
	ServerManager(int ac, char**& av);
	ServerManager(const ServerManager& obj);
	~ServerManager();
	ServerManager& operator=(const ServerManager& obj);
};

#endif
