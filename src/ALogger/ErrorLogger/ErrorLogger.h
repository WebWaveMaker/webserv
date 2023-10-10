#pragma once
#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H

#include <cerrno>
#include "ALogger.hpp"

/**
 * @brief 
 * 
 * @param LOG_ERROR
 * @param LOG_WARN
 * @param LOG_INFO
 * @param LOG_DEBUG
 * 
 */
enum LogLevel { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };

#endif
