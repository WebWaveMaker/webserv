#pragma once
#ifndef ENUM_H
#define ENUM_H

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

enum HttpMethods { GET, POST, DELETE, PUT };

#endif