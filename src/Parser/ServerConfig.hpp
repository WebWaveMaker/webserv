#pragma once
#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "CommonConfig.hpp"	 // Include CommonConfig class
#include "Parser.h"

// Forward declaration of LocationConfig to be used inside ServerConfig
class LocationConfig;

class ServerConfig {
   private:
	CommonConfig* _commonConfig;  // Pointer to CommonConfig

	bool _sendFile;
	int _keepAliveTimeout;
	std::string _defaultType;
	std::pair<std::string, LogLevels> _errorLog;
	std::map<int, std::string> _errorPages;
	int _clientMaxBodySize;
	int _listen;
	std::string _serverName;
	std::string _root;
	bool _autoIndex;
	std::string _index;
	std::vector<HttpMethod> _limitExcept;
	std::vector<LocationConfig> _locations;

	// Private Getters
	bool getSendFile() const;
	int getKeepAliveTimeout() const;
	std::string getDefaultType() const;
	std::pair<std::string, LogLevels> getErrorLog() const;
	std::string getErrorPage(int errorCode) const;
	int getClientMaxBodySize() const;
	int getListen() const;
	std::string getServerName() const;
	std::string getRoot() const;
	bool getAutoIndex() const;
	std::string getIndex() const;
	std::vector<HttpMethod> getLimitExcept() const;

	// Private Setters
	void setSendFile(const std::string& value);
	void setKeepAliveTimeout(const std::string& value);
	void setDefaultType(const std::string& value);
	void setErrorLog(const std::string& value);
	void setErrorPage(const std::string& errorCode, const std::string& errorPage);
	void setClientMaxBodySize(const std::string& value);
	void setListen(const std::string& value);
	void setServerName(const std::string& value);
	void setRoot(const std::string& value);
	void setAutoIndex(const std::string& value);
	void setIndex(const std::string& value);
	void setLimitExcept(const std::string& value);
	void setValueByServerDirectives(ServerDirectives method, const std::string& value);

   public:
	// Constructor & Destructor
	ServerConfig(CommonConfig* commonConfig);
	virtual ~ServerConfig();  // Specify virtual for the destructor

	// Copy constructor & assignment operator
	ServerConfig(const ServerConfig& other);
	ServerConfig& operator=(const ServerConfig& other);

	ConfigValue getValueByServerDirectives(ServerDirectives method) const;
	std::string getErrorPage(int errorCode) const;
	std::vector<LocationConfig> getLocations() const;
};

#endif	// SERVERCONFIG_HPP
