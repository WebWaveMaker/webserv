#pragma once
#ifndef _CONFIG_STORE_HPP
#define _CONFIG_STORE_HPP
#include "Parser.h"
#include "ServerConfig.hpp"

class ConfigStore {
   public:
	ConfigStore();
	virtual ~ConfigStore();
	void setServerConfig(const HttpBlock& httpBlock);
	std::vector<ServerConfig> getServerConfig();

   private:
	std::vector<ServerConfig> _servers;
};

#endif	// _CONFIG_STORE_HPP