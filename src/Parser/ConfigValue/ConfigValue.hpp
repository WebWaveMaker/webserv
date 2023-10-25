#pragma once
#ifndef CONFIGVALUE_HPP
#define CONFIGVALUE_HPP

#include "ErrorLogger.hpp"	// For LogLevels
#include "Parser.h"			// For LogLevels

class ConfigValue {
   public:
	enum ValueType { NONE, BOOL, UINT, STRING, LOG, MEDVEC, STRVEC };

   private:
	ValueType type;

	union Data {
		bool boolean;
		unsigned int integer;
		char* str[sizeof(std::string)];						   // Aligned storage for string
		char* log[sizeof(std::pair<std::string, LogLevels>)];  // Aligned storage for pair
		char* medvec[sizeof(std::vector<HttpMethods>)];		   // Aligned storage for method vector
		char* strvec[sizeof(std::vector<std::string>)];		   // Aligned storage for string vector
	} data;

   public:
	// Constructors and Destructors for each type
	ConfigValue() : type(NONE) {}
	ConfigValue(bool b);
	ConfigValue(unsigned int i);
	ConfigValue(const std::string& s);
	ConfigValue(const std::pair<std::string, LogLevels>& l);
	ConfigValue(const std::vector<HttpMethods>& v);
	ConfigValue(const std::vector<std::string>& v);
	ConfigValue(const ConfigValue& other);
	~ConfigValue();

	// Assignment operators for each type
	ConfigValue& operator=(bool b);
	ConfigValue& operator=(unsigned int i);
	ConfigValue& operator=(const std::string& s);
	ConfigValue& operator=(const std::pair<std::string, LogLevels>& l);
	ConfigValue& operator=(const std::vector<HttpMethods>& v);
	ConfigValue& operator=(const std::vector<std::string>& v);
	ConfigValue& operator=(const ConfigValue& other);

	// Getter functions
	void* asNULL() const;
	bool asBool() const;
	unsigned int asUint() const;
	std::string asString() const;
	std::pair<std::string, LogLevels> asLog() const;
	std::vector<HttpMethods> asMedVec() const;
	std::vector<std::string> asStrVec() const;

	// Check the type
	ValueType getType() const;
};

#endif	// CONFIGVALUE_HPP
