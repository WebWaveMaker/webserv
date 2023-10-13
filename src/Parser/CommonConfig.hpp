#pragma once
#ifndef COMMONCONFIG_HPP
#define COMMONCONFIG_HPP

#include "IConfig.hpp"
#include "Parser.h"

class CommonConfig : public IConfig<HttpDirectives, void*> {
   private:
	std::map<HttpDirectives, ConfigValue> _values;	  // 각 HTTP 메소드에 대응하는 설정 값들을 저장
	std::map<unsigned int, std::string> _errorPages;  // 오류 페이지들을 저장

   public:
	CommonConfig();
	CommonConfig(const CommonConfig& other);
	~CommonConfig();
	CommonConfig& operator=(const CommonConfig& other);

	void setErrorPage(const std::vector<std::string>& values);
	void setDirectives(const std::string& directive, const std::vector<std::string>& values);
	std::string getErrorPage(unsigned int error_code) const;
	ConfigValue getDirectives(HttpDirectives method) const;
};

#endif	// COMMONCONFIG_HPP
