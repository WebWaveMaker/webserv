#pragma once
#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "AConfig.hpp"	// Include IConfig class
#include "ErrorLogger.hpp"
#include "HttpConfig.hpp"  // Include CommonConfig class
#include "LocationConfig.hpp"
#include "Parser.h"
#include "shared_ptr.hpp"
#include "utils.hpp"

class HttpConfig;
class LocationConfig;

class ServerConfig : public AConfig {
   private:
	utils::shared_ptr<HttpConfig> _parent;
	std::map<std::string, utils::shared_ptr<LocationConfig> > _locations;

   public:
	ServerConfig();
	ServerConfig(utils::shared_ptr<HttpConfig> parent);	 // Moved initialization to constructor initializer list
	ServerConfig(const ServerConfig& other);
	~ServerConfig();
	ServerConfig& operator=(const ServerConfig& other);

	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values);
	virtual void setErrorPage(const std::vector<std::string>& values);
	virtual std::string getErrorPage(unsigned int error_code) const;
	virtual ConfigValue getDirectives(Directives method) const;
	void setLocations(std::string identifier, utils::shared_ptr<LocationConfig> location);
	std::string getOwnRoot();
	std::vector<std::string> getOwnIndex();
	bool getOwnConfirmedMethods(Directives method) const;
	utils::shared_ptr<LocationConfig> getLocation(const std::string& identifier);
	utils::shared_ptr<LocationConfig> getLocationConfig(const std::string& reqPath);
	std::string getMimeTypes(const std::string& extension) const;
	bool hasMimeTypes(const std::string& extension) const;
};
#endif	// SERVERCONFIG_HPP
