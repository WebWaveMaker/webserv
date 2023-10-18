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
#define DEF_ERROR_CODE static_cast<unsigned int>(500)

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

enum ValueType { BOOL, UINT, STRING, LOG };

enum Directives {
	SENDFILE,			   // http, server, location	on : true, off : false default : true
	ERROR_PAGE,			   // http, server, location	int, string default : specific to error codes
	KEEPALIVE_TIMEOUT,	   // http, server	int default : 75
	DEFAULT_TYPE,		   // http, server	string default : application/octet-stream
	ERROR_LOG,			   // http, server	string, enum LogLevels default : logs/error.log error
	CLIENT_MAX_BODY_SIZE,  // http, server	int default : 1m
	LISTEN,				   // server, 	int default : 80
	SERVER_NAME,		   // server, 	string default : empty
	ROOT,				   // server, location 	string default : specific to directory
	AUTOINDEX,			   // server, location 	on : true, off : false default : false
	INDEX,				   // server, location 	string default : index.html
	LIMIT_EXCEPT		   // server, location 	std::vector<enum HttpMethod> default : {GET, POST, DELETE, PUT}
};

enum LogLevels { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };

enum HttpMethod { GET, POST, DELETE, PUT };

extern unsigned int stringToDecimal(const std::string& value);

#endif	// _PARSER_H