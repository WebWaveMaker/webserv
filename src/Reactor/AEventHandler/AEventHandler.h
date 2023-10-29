#pragma once
#ifndef AEVENTHANDLER_H
#define AEVENTHANDLER_H

#include <sys/socket.h>
#include "AccessLogger.hpp"
#include "ErrorLogger.hpp"
#include "utils.hpp"

#ifndef handle_t
typedef int handle_t;
#endif

/**
 * @brief 
 * 
 */
enum EventType { EVENT_READ, EVENT_WRITE };

#endif
