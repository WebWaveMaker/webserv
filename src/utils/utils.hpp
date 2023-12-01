#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include "utils.h"

namespace utils {
	std::string itos(const int& num);
	std::string lltos(const long long& num);
	std::string formatTime(const std::time_t t, const char* format);
	std::string getCurTime(const char* format);
	unsigned int stringToDecimal(const std::string& value);
	std::vector<std::string> split(std::string input, const std::string& delimiter);
	unsigned int stoui(const std::string s);
	std::string join(std::vector<std::string>& elems, std::string delim);
	std::string trim(const std::string& source);
	fd_t makeFd(const char* path, const char* option);
	std::string generateRandomString();
	std::string removeSubstring(const std::string& mainStr, const std::string& substr);
	std::string generateRandomHexColor();

	template <typename T, typename S>
	void insertData(T& target, const S& source) {
		target.insert(target.end(), source.begin(), source.end());
		// target.reserve(target.size() + source.size());	// 성능 최적화를 위해 메모리를 미리 할당
		// for (typename S::const_iterator it = source.begin(); it != source.end(); ++it) {
		// 	target.push_back(*it);
		// }
	}

	template <typename T>
	T toHexNum(const std::string& str) {
		T num;
		std::istringstream ss(str);

		if (!(ss >> std::hex >> num)) {
			throw(std::invalid_argument("String is not representing hex: " + str));
		}

		char remaining;
		if (ss >> remaining) {
			throw(std::invalid_argument("Leftover character after str to hex convertion: " + str));
		}
		return (num);
	}
}  // namespace utils

#endif
