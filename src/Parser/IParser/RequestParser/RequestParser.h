#pragma once
#ifndef REQUESTPARSER_H
#define REQUESTPARSER_H

#include <queue>
#include <sstream>
#include <utility>
#include "ErrorLogger.hpp"
#include "HttpMessage.hpp"
#include "IParser.hpp"
#include "ServerConfig.hpp"
#include "const.h"
#include "enum.h"
#include "nullptr_t.hpp"
#include "shared_ptr.hpp"
#include "utils.hpp"

typedef utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> > request_t;

#endif
