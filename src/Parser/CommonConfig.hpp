#pragma once
#ifndef COMMONCONFIG_HPP
#define COMMONCONFIG_HPP

#include "ConfigValue.hpp"
#include "Parser.h"

class CommonConfig {
   private:
	ConfigValue _sendFile;
	ConfigValue _keepAliveTimeout;
	ConfigValue _defaultType;
	ConfigValue _clientMaxBodySize;
	ConfigValue _errorLog;
	std::map<unsigned int, std::string> _errorPages;  // 오류 페이지들을 저장

	// 각 private 맴버 변수에 대응하는 getter 함수들
	bool getSendFile() const;
	unsigned int getKeepAliveTimeout() const;
	std::string getDefaultType() const;
	unsigned int getClientMaxBodySize() const;

	// 각 private 맴버 변수에 대응하는 setter 함수들
	void setSendFile(const std::string& value);
	void setKeepAliveTimeout(const std::string& value);
	void setDefaultType(const std::string& value);
	void setClientMaxBodySize(const std::string& value);

   public:
	CommonConfig();
	CommonConfig(const CommonConfig& other);
	virtual ~CommonConfig();
	CommonConfig& operator=(const CommonConfig& other);

	void setErrorLog(const std::string& logPath, const std::string& logLevel);
	void setErrorPage(const std::vector<std::string>& values);
	void setValueByHttpMethod(const std::string& directive, const std::vector<std::string>& values);
	std::pair<std::string, LogLevels> getErrorLog() const;
	std::string getErrorPage(unsigned int error_code) const;
	ConfigValue getValueByHttpMethod(HttpDirectives method) const;
};

#endif	// COMMONCONFIG_HPP
