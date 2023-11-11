#pragma once
#ifndef DEFAULTRESPONSEBUILDER_HPP
#define DEFAULTRESPONSEBUILDER_HPP

#include "DefaultResponseBuilder.h"

class DefaultResponseBuilder : public utils::TSingleton<DefaultResponseBuilder> {
   private:
	std::map<std::string, std::string> _httpStatusCodes;

	void initHttpStatusCode();

   public:
	DefaultResponseBuilder();
	~DefaultResponseBuilder();

	std::vector<std::string> setDefaultStartLine(const int statusCode);
	std::map<std::string, std::string> setDefaultHeader(const utils::shared_ptr<ServerConfig>& serverConfig);
};

#endif
