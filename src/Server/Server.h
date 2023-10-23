#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <exception>
#include <iostream>
#include <map>
#include <vector>

#include "AccessLogger.hpp"
#include "Client.hpp"
#include "Dispatcher.hpp"
#include "ErrorLogger.hpp"
#include "ICallback.hpp"
#include "ServerConfig.hpp"
#include "ServerEventHandler.hpp"
#include "TSingleton.hpp"

class ServerConfig;
class Client;
class ServerEventHandler;
class AccessLogger;
class ErrorLogger;
class ClientEventHandler;

enum EventType;

#endif
