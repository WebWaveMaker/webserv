#pragma once
#ifndef HTTPCONFIG_HPP
#define HTTPCONFIG_HPP

#include "HttpConfig.h"

class HttpConfig : public AConfig {
   private:
	utils::shared_ptr<Mime> _mimeTypes;

   public:
	HttpConfig();
	HttpConfig(const HttpConfig& other);
	~HttpConfig();
	HttpConfig& operator=(const HttpConfig& other);
	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values);
	virtual void setErrorPage(const std::vector<std::string>& values);
	virtual std::string getErrorPage(unsigned int error_code) const;
	virtual ConfigValue getDirectives(Directives method) const;

	std::string getMimeTypes(const std::string& extension) const;

	void setMimeTypes(utils::shared_ptr<Mime> mimeTypes);

	bool hasMimeTypes(const std::string& extension) const;
};

#endif	// HTTPCONFIG_HPP
