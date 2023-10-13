#include "ConfigStore.hpp"

ConfigStore::ConfigStore() {}

ConfigStore::ConfigStore(const CommonConfig& common) : _common(common) {}

ConfigStore::ConfigStore(const ConfigStore& obj) {
	*this = obj;
}

ConfigStore::~ConfigStore() {}

ConfigStore& ConfigStore::operator=(const ConfigStore& obj) {
	if (this != &obj) {
		_common = obj._common;
		// _servers = obj._servers;
	}
	return *this;
}

// void ConfigStore::addServer(const ServerConfig& server) {
// 	_servers.push_back(server);
// }

CommonConfig& ConfigStore::getCommonConfig() {
	return _common;
}

// std::vector<ServerConfig>& ConfigStore::getServers() {
// 	return _servers;
// }
