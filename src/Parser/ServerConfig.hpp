#pragma once
#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "CommonConfig.hpp"	 // Include CommonConfig class
#include "IConfig.hpp"		 // Include IConfig class
#include "Parser.h"

class ServerConfig : public IConfig<ServerDirectives, CommonConfig> {
   private:
	std::map<ServerDirectives, ConfigValue> _values;  // 각 서버에 대응하는 설정 값들을 저장
	std::map<unsigned int, std::string> _errorPages;  // 오류 페이지들을 저장
	CommonConfig& _commonConfig;

   public:
	ServerConfig(CommonConfig& commonConfig);
	ServerConfig(const ServerConfig& other);
	~ServerConfig();
	ServerConfig& operator=(const ServerConfig& other);

	void setErrorPage(const std::vector<std::string>& values);
	void setDirectives(const std::string& directive, const std::vector<std::string>& values);
	ConfigValue getDirectives(ServerDirectives method) const;
	std::string getErrorPage(unsigned int error_code) const;
};

#endif	// SERVERCONFIG_HPP
