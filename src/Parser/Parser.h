#pragma once
#ifndef _PARSER_H
#define _PARSER_H

#include <fstream>
#include <iostream>
#include <map>
#include <new>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "enum.h"

#define DEF_SENDFILE bool(true)
#define DEF_KEEPALIVE_TIMEOUT static_cast<unsigned int>(75)
#define DEF_DEFAULT_TYPE "application/octet-stream"
#define DEF_ERROR_LOG_PATH "logs/error.log"
#define DEF_CLIENT_MAX_BODY_SIZE static_cast<unsigned int>(1048576)	 // 1m
#define DEF_LISTEN static_cast<unsigned int>(80)
#define DEF_SERVER_NAME ""
#define DEF_ROOT ""
#define DEF_AUTOINDEX bool(false)
#define DEF_INDEX "index.html"
#define DEF_ERROR_PAGE "error.html"
#define DEF_ERROR_CODE static_cast<unsigned int>(INTERNAL_SERVER_ERROR)

struct Directive {
	std::string name;
	std::vector<std::string> parameters;
};

struct LocationBlock {
	std::vector<Directive> directives;
	std::string identifier;
};

struct ServerBlock {
	std::vector<Directive> directives;
	std::vector<LocationBlock> locations;
};

struct HttpBlock {
	std::vector<Directive> directives;
	std::vector<ServerBlock> servers;
};

#endif	// _PARSER_H