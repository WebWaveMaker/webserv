#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include "utils.h"

namespace utils {
	std::string itos(const int& num);
	std::string getCurTime(const char* format);
	unsigned int stringToDecimal(const std::string& value);

}  // namespace utils

#endif
