/**
 * @file main.cpp
 * @brief 
 * @author WebWaveMaker
 * @version 0.1
 * @date 2023-10-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <cerrno>
#include "ConfigParser.hpp"
#include "Dispatcher.hpp"
#include "ServerConfig.hpp"
#include "ServerManager.hpp"

int main(int ac, char** av) {
	if (ac != 2) {
		std::cerr << "Too Many Arguments\n";
		return (EXIT_FAILURE);
	}

	std::string configFile = av[1];
	std::vector<ServerConfig*>* serverConfigs = new std::vector<ServerConfig*>;
	ConfigParser configParser;
	ServerManager* serverManager = utils::nullptr_t;

	try {
		bool parse = configParser.parse(configFile, *serverConfigs);
		serverManager = new ServerManager(serverConfigs);
		if (parse == false)
			return EXIT_FAILURE;
		std::cout << "here\n";
		while (true)
			reactor::Dispatcher::getInstance()->handleEvent();
	} catch (std::exception& e) {
		std::cerr << e.what() << "\n";
		if (serverManager)
			delete serverManager;
		return (EXIT_FAILURE);
	}

	if (serverManager)
		delete serverManager;
	return (EXIT_SUCCESS);
}
