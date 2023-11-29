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
	INDEX,				   // server, location 	std::vector<string> default : index.html
	LIMIT_EXCEPT,		   // server, location 	std::vector<enum HttpMethods> default : {GET, POST, DELETE, PUT}
	RETURN,				   // server, location 	int, string default : empty
	CGI_INDEX,			   // server, location 	std::vector<string> default : empty
};

enum HTTP_STATUS {
	// status error
	MY_ERROR = -1,
	// 200
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NO_CONTENT = 204,  // body가 없는 경우
	PARTIAL_CONTENT = 206,
	// 300
	MULTIPLE_CHOICES = 300,
	MOVED_PERMANENTLY = 301,
	FOUND = 302,
	SEE_OTHER = 303,
	NOT_MODIFIED = 304,
	TEMPORARY_REDIRECT = 307,
	PERMANENT_REDIRECT = 308,

	// 400
	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	REQUEST_TIMEOUT = 408,
	CONFLICT = 409,
	GONE = 410,
	LENGTH_REQUIRED = 411,
	PAYLOAD_TOO_LARGE = 413,
	UNSUPPORTED_MEDIA_TYPE = 415,
	TOO_MANY_REQUESTS = 429,

	// 500
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAILABLE = 503,
	GATEWAY_TIMEOUT = 504,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

enum LogLevels { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };

enum HttpMethods { GET, HEAD, POST, DELETE, PUT, UNKNOWN };

enum AsyncState { PENDING, RESOLVE, TERMINATE, ACCEPT, NONE };

enum HttpMessageState {
	DONE,
	HTTP_ERROR,
	START_LINE,
	HEADER,
	BODY,
	CHUNKED_FIRST,
	CHUNKED,
	CHUNKED_DONE,
	CHUNKED_ERROR,
	LONG_FIRST,
	LONG_BODY,
	LONG_BODY_DONE,
	LONG_BODY_ERROR
};

enum FileMode { MODE_ERROR, MODE_FILE, MODE_DIRECTORY };

enum EventType {
	EVENT_READ = -1,
	EVENT_WRITE = -2,
	// EVFILT_AIO = -3,
	// EVFILT_VNODE = -4,
	// EVFILT_PROC = -5,
	// EVFILT_SIGNAL = -6,
	EVENT_TIMER = -7,
	EVENT_EXE = -11
	// EVFILT_MACHPORT = -8,
	// EVFILT_FS = -9,
	// EVFILT_USER = -10,
	// EVFILT_VM = -12,
	// EVFILT_EXCEPT = -15
};

#endif
