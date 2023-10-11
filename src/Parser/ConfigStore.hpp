#pragma once
#ifndef CONFIGSTORE_HPP
#define CONFIGSTORE_HPP

#include <vector>
#include "ServerConfig.hpp"

class ConfigStore {
   private:
	std::vector<ServerConfig> _servers;

   public:
	ConfigStore();
	ConfigStore(const ConfigStore& obj);
	~ConfigStore();
	ConfigStore& operator=(const ConfigStore& obj);
};
