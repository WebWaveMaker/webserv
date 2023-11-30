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

int main(int ac, char** av, char** envp) {
	if (ac != 2)
		return ErrorLogger::parseError(__FILE__, __LINE__, __func__, "wrong arguments");
	try {
		ServerManager::getInstance()->init(av[1], envp);

		while (true)
			reactor::Dispatcher::getInstance()->handleEvent();
	} catch (...) {
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
