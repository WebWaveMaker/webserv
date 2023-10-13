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
#include "ServerManager.hpp"

int main(int ac, char** av) {
	try {
		ServerManager serverManager(ac, av);
	} catch (...) {
		return errno;
	}
	return EXIT_SUCCESS;
}