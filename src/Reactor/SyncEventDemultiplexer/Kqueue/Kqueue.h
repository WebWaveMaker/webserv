#pragma once
#ifndef KQUEUE_H
#define KQUEUE_H

#include <sys/event.h>
#include "ErrorLogger.hpp"
#include "nullptr.hpp"

typedef int fd_t;

/**
 * @brief 
 * 
 */
enum FdSetType { CLIENT_FD, SERVER_FD };

#endif
