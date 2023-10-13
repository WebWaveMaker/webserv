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
#define DEF_DEFAULT_TYPE std::string("application/octet-stream")
#define DEF_ERROR_LOG_PATH std::string("logs/error.log")
#define DEF_ERROR_LOG_LEVEL LogLevels(LOG_ERROR)
#define DEF_CLIENT_MAX_BODY_SIZE static_cast<unsigned int>(1048576)	 // 1m
#define DEF_LISTEN static_cast<unsigned int>(80)
#define DEF_SERVER_NAME std::string("")
#define DEF_ROOT std::string("")
#define DEF_AUTOINDEX bool(false)
#define DEF_INDEX std::string("index.html")
#define DEF_LIMIT_EXCEPT std::vector<HttpMethod>({GET, POST, DELETE, PUT})
#define DEF_ERROR_PAGE std::string("error.html")
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

enum HttpDirectives {
	HTTP_SENDFILE,			   // on : true, off : false default : true
	HTTP_KEEPALIVE_TIMEOUT,	   // int default : 75
	HTTP_DEFAULT_TYPE,		   // string default : application/octet-stream
	HTTP_ERROR_LOG,			   // string, enum LogLevels default : logs/error.log error
	HTTP_ERROR_PAGE,		   // int, string default : specific to error codes
	HTTP_CLIENT_MAX_BODY_SIZE  // int default : 1m
};

enum ServerDirectives {
	SERVER_SENDFILE,			  // on : true, off : false default : true
	SERVER_KEEPALIVE_TIMEOUT,	  // int default : 75
	SERVER_DEFAULT_TYPE,		  // string default : application/octet-stream
	SERVER_ERROR_LOG,			  // string, enum LogLevels default : logs/error.log error
	SERVER_ERROR_PAGE,			  // int, string default : specific to error codes
	SERVER_CLIENT_MAX_BODY_SIZE,  // int default : 1m
	SERVER_LISTEN,				  // int default : 80
	SERVER_SERVER_NAME,			  // string default : empty
	SERVER_ROOT,				  // string default : specific to directory
	SERVER_AUTOINDEX,			  // on : true, off : false default : false
	SERVER_INDEX,				  // string default : index.html
	SERVER_LIMIT_EXCEPT			  // std::vector<enum HttpMethod> default : {GET, POST, DELETE, PUT}
};

enum LocationDirectives {
	LOC_SENDFILE,	   // on : true, off : false default : true
	LOC_ERROR_PAGE,	   // int, string default : specific to error codes
	LOC_LIMIT_EXCEPT,  // std::vector<enum HttpMethod> default : {GET, POST, DELETE, PUT}
	LOC_ROOT,		   // string default : specific to directory
	LOC_AUTOINDEX,	   // on : true, off : false default : false
	LOC_INDEX		   // string default : index.html
};

enum LogLevels { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };

enum HttpMethod { GET, POST, DELETE, PUT };

extern unsigned int stringToDecimal(const std::string& value);

#endif	// _PARSER_H