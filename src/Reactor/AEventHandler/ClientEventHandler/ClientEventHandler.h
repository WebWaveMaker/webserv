#pragma once
#ifndef CLIENTEVENTHANDLER_H
#define CLIENTEVENTHANDLER_H

#include <unistd.h>	 // For read()
#include <iostream>	 // For std::cout
#include <vector>	 // For std::vector

#include "Client.hpp"
#include "Dispatcher.hpp"
#include "ICallback.hpp"
#include "AEventHandler.hpp"
#include "RequestParser.hpp"

#define BUFFER_SIZE 1024

#endif
