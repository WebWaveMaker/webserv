// ConfigParser.h
#pragma once
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "ErrorLogger.hpp"
#include "Parser.h"
#include "ServerConfig.hpp"

class ConfigParser {
   public:
	ConfigParser();
	virtual ~ConfigParser();
	virtual bool parse(const std::string& filename, std::vector<ServerConfig*>& http);

   private:
	std::string parser(const std::string& filename);
	bool httpConfigParser(const HttpBlock& http, HttpConfig* httpConfig);
	bool serverConfigParser(const ServerBlock& serverBlock, ServerConfig* serverConfig);
	bool locationConfigParser(const LocationBlock& locationBlock, LocationConfig* locationConfig);
	bool match(const std::string& content, size_t& position, const std::string& expected);
	void skipWhitespace(const std::string& content, size_t& position);
	bool httpBlockTokenizer(const std::string& content, size_t& position, HttpBlock& httpBlock);
	bool serverBlockTokenizer(const std::string& content, size_t& position, ServerBlock& serverBlock);
	bool locationBlockTokenizer(const std::string& content, size_t& position, LocationBlock& locationBlock);
	bool directiveTokenizer(const std::string& content, size_t& position, Directive& directive);
};

#endif	// CONFIG_PARSER_H