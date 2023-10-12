#pragma once
#ifndef ALOGGER_H
#define ALOGGER_H

#include <unistd.h>
#include <ctime>
#include <iostream>
#include "color.h"
#include "utils.hpp"

namespace logTimeFormat {
	static const char* accessTimeFormat = " [%d/%b/%Y:%H:%M:%S] ";
	static const char* errorTimeFormat = " %Y/%m/%d %H:%M:%S ";
	static const char* systemTimeFormat = " %a, %d %b %Y %H:%M:%S ";
};	// namespace logTimeFormat

#endif