#pragma once
#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include "ErrorLogger.hpp"
#include "Parser.h"
#include "ServerConfig.hpp"
#include "shared_ptr.hpp"
#include "utils.hpp"

class ServerConfig;

class LocationConfig : public AConfig {
   private:
	utils::shared_ptr<ServerConfig> _parent;

   public:
	LocationConfig();
	LocationConfig(utils::shared_ptr<ServerConfig> parent);	 // Moved initialization to constructor initializer list
	LocationConfig(const LocationConfig& other);
	~LocationConfig();
	LocationConfig& operator=(const LocationConfig& other);

	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values);
	virtual void setErrorPage(const std::vector<std::string>& values);
	virtual std::string getErrorPage(unsigned int error_code) const;
	virtual ConfigValue getDirectives(Directives method) const;
	utils::shared_ptr<ServerConfig> getParent();
	bool getOwnRoot(std::string& str);
	bool getOwnIndex(std::vector<std::string>& vec);
	bool isCgi();
	bool isRedirect();
	std::string getMimeTypes(const std::string& extension) const;
};

#endif	// LOCATIONCONFIG_HPP
