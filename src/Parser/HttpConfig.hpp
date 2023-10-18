#pragma once
#ifndef HTTPCONFIG_HPP
#define HTTPCONFIG_HPP

#include "AConfig.hpp"
#include "Parser.h"

class HttpConfig : public AConfig {
   public:
	HttpConfig();
	HttpConfig(const HttpConfig& other);
	~HttpConfig();
	HttpConfig& operator=(const HttpConfig& other);

	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values);
	virtual void setErrorPage(const std::vector<std::string>& values);
	virtual std::string getErrorPage(unsigned int error_code) const;
	virtual ConfigValue getDirectives(Directives method) const;
};

#endif	// HTTPCONFIG_HPP