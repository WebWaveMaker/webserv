#pragma once
#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "ErrorLogger.hpp"
#include "IParser.hpp"
#include "Parser.h"
#include "ServerConfig.hpp"
#include "types.h"

typedef std::vector<utils::shared_ptr<ServerConfig> > config_t;

#endif
