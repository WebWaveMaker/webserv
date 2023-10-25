#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include "utils.h"

namespace utils {
	std::string itos(const int& num);
	unsigned int stringToDecimal(const std::string& value);
	std::string getCurTime(const char* format);

}  // namespace utils

#endif
