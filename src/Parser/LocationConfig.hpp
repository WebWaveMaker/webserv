#pragma once
#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include "Parser.h"
#include "ServerConfig.hpp"

class LocationConfig {
   private:
	ServerConfig* _serverConfig;  // Pointer to ServerConfig

	bool _sendFile;
	std::map<int, std::string> _errorPages;
	std::vector<HttpMethod> _limitExcept;
	std::string _root;
	bool _autoIndex;
	std::string _index;

	// Private Getters
	bool getSendFile() const;
	std::vector<HttpMethod> getLimitExcept() const;
	std::string getRoot() const;
	bool getAutoIndex() const;
	std::string getIndex() const;

	// Private Setters
	void setSendFile(const std::string& value);
	void setErrorPage(const std::string& errorCode, const std::string& errorPage);
	void setLimitExcept(const std::string& value);
	void setRoot(const std::string& value);
	void setAutoIndex(const std::string& value);
	void setIndex(const std::string& value);
	void setValueByLocationDirectives(LocationDirectives method, const std::string& value);

   public:
	LocationConfig(ServerConfig* serverConfig);
	virtual ~LocationConfig();	// Specify virtual for the destructor

	// Copy constructor & assignment operator
	LocationConfig(const LocationConfig& other);
	LocationConfig& operator=(const LocationConfig& other);

	std::string getErrorPage(int errorCode) const;
	ConfigValue getValueByLocationDirectives(LocationDirectives method) const;
};

#endif	// LOCATIONCONFIG_HPP
