#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include "utils.h"

namespace utils {
	std::string itos(const int& num);
	std::string lltos(const long long& num);
	std::string getCurTime(const char* format);
	unsigned int stringToDecimal(const std::string& value);
	std::vector<std::string> split(std::string input, const std::string& delimiter);
	unsigned int stoui(const std::string s);
	std::string join(std::vector<std::string>& elems, std::string delim);
	std::string trim(const std::string& source);
	fd_t makeFd(const char* path, const char* option);
}  // namespace utils

#endif
