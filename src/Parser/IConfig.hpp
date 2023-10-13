
#pragma once
#ifndef ICONFIG_HPP
#define ICONFIG_HPP

#include "ConfigValue.hpp"
#include "Parser.h"	 // For LogLevels
//--std=98
template <typename T, typename T2>
class IConfig {
   protected:
	std::map<T, ConfigValue> _directives = {};			   // 지시어를 저장
	std::map<unsigned int, std::string> _errorPages = {};  // 에러 페이지를 저장
	T2 _block = 0;										   // vector<server> or map<std::string, location>

	ConfigValue addBooleanValue(const std::string& value) {
		if (value == "on") {
			return ConfigValue(true);
		}
		if (value == "off") {
			return ConfigValue(false);
		}
		throw std::runtime_error("Invalid boolean value");
	}
	ConfigValue addUnsignedIntValue(const std::string& value) {
		unsigned int unsignedValue = static_cast<unsigned int>(stringToDecimal(value));
		if (unsignedValue == 0) {
			throw std::runtime_error("Invalid unsigned int value");
		}
		return ConfigValue(unsignedValue);
	}
	ConfigValue addStringValue(const std::string& value) { return ConfigValue(value); }
	ConfigValue addLogValue(const std::vector<std::string>& value) {
		LogLevels level;
		if (value[1] == "debug") {
			level = LOG_DEBUG;
		} else if (value[1] == "info") {
			level = LOG_INFO;
		} else if (value[1] == "warn") {
			level = LOG_WARN;
		} else if (value[1] == "error") {
			level = LOG_ERROR;
		} else {
			throw std::runtime_error("Invalid log level");
		}
		return ConfigValue(std::make_pair(value[0], level));
	}

   public:
	IConfig() {}
	virtual ~IConfig() = 0;

	// 순수 가상 함수들
	virtual void setErrorPage(const std::vector<std::string>& values) = 0;
	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values) = 0;
	virtual std::string getErrorPage(unsigned int error_code) const = 0;
	virtual ConfigValue getDirectives(T method) const = 0;

	void setBlock(T2 block);
	T2 getBlock() const;
};

// 순수 가상 소멸자의 구현
template <typename T, typename T2>
IConfig<T, T2>::~IConfig() {}

#endif	// ICONFIG_HPP
