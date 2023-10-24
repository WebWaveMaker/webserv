#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <algorithm>
#include "color.h"

namespace logTimeFormat {
	static const char* accessTimeFormat = " [%d/%b/%Y:%H:%M:%S] ";
	static const char* errorTimeFormat = " %Y/%m/%d %H:%M:%S ";
	static const char* systemTimeFormat = " %a, %d %b %Y %H:%M:%S ";
	static const char* GMTtimeFormat = "%a, %d %b %Y %H:%M:%S GMT";
	static const char* UTCtimeFormat = "%Y-%m-%dT%H:%M:%SZ";
};	// namespace logTimeFormat

#endif
