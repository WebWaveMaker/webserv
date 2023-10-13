#pragma once
#ifndef CONFIGSTORE_HPP
#define CONFIGSTORE_HPP

#include "CommonConfig.hpp"
#include "Parser.h"
#include "ServerConfig.hpp"

class ConfigStore {
   private:
	CommonConfig _common;
	// std::vector<ServerConfig> _servers;

   public:
	ConfigStore();
	ConfigStore(const CommonConfig& common);
	ConfigStore(const ConfigStore& obj);
	~ConfigStore();
	ConfigStore& operator=(const ConfigStore& obj);

	// void addServer(const ServerConfig& server);
	CommonConfig& getCommonConfig();
	// std::vector<ServerConfig>& getServers();
};

#endif	// CONFIGSTORE_HPP