// ConfigParser.h
#pragma once
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "ConfigParser.h"

class ConfigParser : IParser<config_t> {
   public:
	ConfigParser();
	virtual ~ConfigParser();
	virtual config_t parse(const std::string& content);

   private:
	std::string parser(const std::string& filename);
	bool httpConfigParser(const HttpBlock& http, HttpConfig* httpConfig);
	bool serverConfigParser(const ServerBlock& serverBlock, utils::shared_ptr<ServerConfig> serverConfig);
	bool locationConfigParser(const LocationBlock& locationBlock, utils::shared_ptr<LocationConfig> locationConfig);
	bool match(const std::string& content, size_t& position, const std::string& expected);
	void skipWhitespace(const std::string& content, size_t& position);
	bool httpBlockTokenizer(const std::string& content, size_t& position, HttpBlock& httpBlock);
	bool serverBlockTokenizer(const std::string& content, size_t& position, ServerBlock& serverBlock);
	bool locationBlockTokenizer(const std::string& content, size_t& position, LocationBlock& locationBlock);
	bool directiveTokenizer(const std::string& content, size_t& position, Directive& directive);
};

#endif	// CONFIG_PARSER_H