#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>	// for std::memset
#include <iostream>
#include <map>
#include <stdexcept>  // for std::runtime_error

#include "AccessLogger.hpp"
#include "Client.hpp"
#include "ErrorLogger.hpp"
#include "ServerAcceptHandlerFactory.hpp"
#include "ServerConfig.hpp"

#endif
