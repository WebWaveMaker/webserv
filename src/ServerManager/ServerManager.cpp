#include "ServerManager.hpp"

ServerManager::ServerManager() {}
ServerManager::ServerManager(int ac, char**& av) {
	(void)av;
	switch (ac) {
		case 1:
			// parsing default.conf
			break;

		case 2:
			// parsing av[1];
			break;

		default:
			// error
			break;
	}
};
// ServerManager::ServerManager(const ServerManager& obj) {}
ServerManager::~ServerManager() {}
// ServerManager& ServerManager::operator=(const ServerManager& obj) {}