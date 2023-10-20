#pragma once
#ifndef RequestParser_H
#define RequestParser_H

#include <queue>
#include <sstream>
#include <utility>
#include "HttpMessage.hpp"
#include "IParser.hpp"
#include "const.h"
#include "enum.h"
#include "nullptr_t.hpp"
#include "shared_ptr.hpp"
#include "utils.hpp"

typedef utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> > request_t;

#endif
