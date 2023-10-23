#pragma once
#ifndef SERVEREVENTHANDLER_H
#define SERVEREVENTHANDLER_H

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
// #include "ClientEventHandler.hpp"
#include "Client.hpp"
#include "ErrorLogger.hpp"
#include "IEventHandler.hpp"

class Client;
class AccessLogger;
class ErrorLogger;
class ClientEventhandler;

#endif
