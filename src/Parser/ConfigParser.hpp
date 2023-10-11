// ConfigParser.h
#pragma once
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "ConfigStore.hpp"
#include "IParser.hpp"

class ConfigParser : public IParser<ConfigStore> {
   public:
	ConfigParser();
	virtual ~ConfigParser();
	virtual bool parse(const std::string& filename, ConfigStore& configStore) override;

   private:
	bool match(const std::string& content, size_t& position, const std::string& expected);
	void skipWhitespace(const std::string& content, size_t& position);
	bool httpBlockTokenizer(const std::string& content, size_t& position, HttpBlock& httpBlock);
	bool serverBlockTokenizer(const std::string& content, size_t& position, ServerBlock& serverBlock);
	bool locationBlockTokenizer(const std::string& content, size_t& position, LocationBlock& locationBlock);
	bool directiveTokenizer(const std::string& content, size_t& position, Directive& directive);
};

#endif	// CONFIG_PARSER_H