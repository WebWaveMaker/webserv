#pragma once
#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H

#include <sys/types.h>
#include <cerrno>
#include "ALogger.hpp"
#include "enum.h"

#ifndef errno_t
typedef int errno_t;
#endif

#endif
