#pragma once
#ifndef CLIENTEVENTHANDLER_H
#define CLIENTEVENTHANDLER_H

#include <unistd.h>	 // For read()
#include <iostream>	 // For std::cout
#include <vector>	 // For std::vector

#include "AEventHandler.hpp"
#include "Client.hpp"
#include "Dispatcher.hpp"
#include "ICallback.hpp"
#include "RequestParser.hpp"

#define BUFFER_SIZE 1024

#endif
