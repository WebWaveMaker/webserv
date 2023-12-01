#pragma once
#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include "AConfig.h"

class AConfig {
   protected:
	std::map<Directives, ConfigValue> _directives;
	std::map<unsigned int, std::string> _errorPages;
	ConfigValue addBooleanValue(const std::string& value);
	ConfigValue addUnsignedIntValue(const std::string& value);
	ConfigValue addStringValue(const std::string& value);
	ConfigValue addLogValue(const std::vector<std::string>& value);
	ConfigValue addMedVecValue(const std::vector<HttpMethods>& value);
	ConfigValue addStrVecValue(const std::vector<std::string>& value);

   public:
	AConfig();
	virtual ~AConfig();
	AConfig(const AConfig& other);
	AConfig& operator=(const AConfig& other);
	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values) = 0;
	virtual void setErrorPage(const std::vector<std::string>& values) = 0;
	virtual std::string getErrorPage(unsigned int error_code) const = 0;
	virtual ConfigValue getDirectives(Directives method) const = 0;
	bool isDirective(Directives method) const;
};

#endif	// ACONFIG_HPP
