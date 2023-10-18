#pragma once
#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include "ConfigValue.hpp"
#include "Parser.h"	 // For LogLevels

// serverConfid, locationConfig, httpConfig의 부모 클래스
//
class AConfig {
   protected:
	std::map<Directives, ConfigValue> _directives;	  // 지시어를 저장
	std::map<unsigned int, std::string> _errorPages;  // 에러 페이지를 저장

	ConfigValue addBooleanValue(const std::string& value);
	ConfigValue addUnsignedIntValue(const std::string& value);
	ConfigValue addStringValue(const std::string& value);
	ConfigValue addLogValue(const std::vector<std::string>& value);
	ConfigValue addMedVecValue(const std::vector<HttpMethod>& value);
	ConfigValue addStrVecValue(const std::vector<std::string>& value);

   public:
	AConfig();	// Moved initialization to constructor initializer list
	virtual ~AConfig();
	AConfig(const AConfig& other);
	AConfig& operator=(const AConfig& other);
	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values) = 0;
	virtual void setErrorPage(const std::vector<std::string>& values) = 0;
	virtual std::string getErrorPage(unsigned int error_code) const = 0;
	virtual ConfigValue getDirectives(Directives method) const = 0;
};

#endif	// ACONFIG_HPP
