#pragma once
#ifndef GETRESPONSEBUILDER_H
#define GETRESPONSEBUILDER_H

#include <dirent.h>
#include "DefaultResponseBuilder.hpp"
#include "ErrorResponseBuilder.hpp"
#include "FileReadHandlerFactory.hpp"
#include "HttpMessage.hpp"
#include "IBuilder.hpp"
#include "PipeReadHandlerFactory.hpp"
#include "RedirectResponseBuilder.hpp"
#include "RequestParser.h"
#include "ServerConfig.hpp"
#include "SharedData.hpp"
#include "shared_ptr.hpp"
#include "utils.hpp"

#define LIST_HTML_HEADER(path)                                                                        \
	"<html>\r\n<head><title>Index of " + path + "</title></head>\r\n<body>\r\n<h1>Index of " + path + \
		"</h1><hr><pre>" + "<a href=\"../\">../</a>\r\n";

#endif
