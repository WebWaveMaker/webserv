#pragma once
#ifndef SERVERACCEPTHANDLER_H
#define SERVERACCEPTHANDLER_H

#include <arpa/inet.h>	 // For sockaddr_in
#include <fcntl.h>		 // For fcntl()
#include <sys/socket.h>	 // For accept()
#include <unistd.h>		 // For close()
#include <exception>	 // For std::exception
#include <iostream>		 // For std::cout
#include <map>			 // For std::map
#include <stdexcept>	 // For std::runtime_error

#include "AEventHandler.hpp"  // Assuming the path for AEventHandler
#include "AccessLogger.hpp"	  // For AccessLogger
// #include "ClientEventHandler.hpp"  // For ClientEventHandler
#include "ClientReadHandler.hpp"
#include "Dispatcher.hpp"
#include "ErrorLogger.hpp"	// For ErrorLogger
#include "ICallback.hpp"	// For ICallback

#endif
