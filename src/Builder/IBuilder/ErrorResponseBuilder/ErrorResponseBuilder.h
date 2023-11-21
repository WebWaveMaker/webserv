#pragma once
#ifndef ERRORRESPONSEBUILDER_H
#define ERRORRESPONSEBUILDER_H

#include <sys/stat.h>
#include "DefaultResponseBuilder.hpp"
#include "FileReadHandlerFactory.hpp"
#include "HttpMessage.hpp"
#include "IBuilder.hpp"
#include "SharedData.hpp"

#define FINAL_ERROR_PAGE                                                                \
	"<!DOCTYPE html>\n"                                                                 \
	"<html lang=\"en\">\n"                                                              \
	"<head>\n"                                                                          \
	"    <meta charset=\"UTF-8\">\n"                                                    \
	"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"  \
	"    <title>Error Page</title>\n"                                                   \
	"    <style>\n"                                                                     \
	"        body {\n"                                                                  \
	"            font-family: Arial, sans-serif;\n"                                     \
	"            background-color: #f4f4f4;\n"                                          \
	"            color: #333;\n"                                                        \
	"            margin: 40px;\n"                                                       \
	"            padding: 20px;\n"                                                      \
	"            text-align: center;\n"                                                 \
	"        }\n"                                                                       \
	"\n"                                                                                \
	"        h1 {\n"                                                                    \
	"            color: #cc0000;\n"                                                     \
	"        }\n"                                                                       \
	"    </style>\n"                                                                    \
	"</head>\n"                                                                         \
	"<body>\n"                                                                          \
	"    <h1>Error Occurred!</h1>\n"                                                    \
	"    <p>Sorry, an error occurred while trying to display the requested page.</p>\n" \
	"    <p>Please try again later.</p>\n"                                              \
	"</body>\n"                                                                         \
	"</html>\n"

#endif
