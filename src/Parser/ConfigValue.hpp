#pragma once
#ifndef CONFIGVALUE_HPP
#define CONFIGVALUE_HPP

#include "Parser.h"	 // For LogLevels

class ConfigValue {
   public:
	enum ValueType { BOOL, UINT, STRING, LOG };

   private:
	ValueType type;

	union Data {
		bool boolean;
		unsigned int integer;
		char* str[sizeof(std::string)];						   // Aligned storage for string
		char* log[sizeof(std::pair<std::string, LogLevels>)];  // Aligned storage for pair
	} data;

   public:
	// Constructors and Destructors for each type
	ConfigValue(bool b);
	ConfigValue(unsigned int i);
	ConfigValue(const std::string& s);
	ConfigValue(const std::pair<std::string, LogLevels>& l);
	ConfigValue(const ConfigValue& other);
	~ConfigValue();

	// Assignment operators for each type
	ConfigValue& operator=(bool b);
	ConfigValue& operator=(unsigned int i);
	ConfigValue& operator=(const std::string& s);
	ConfigValue& operator=(const std::pair<std::string, LogLevels>& l);
	ConfigValue& operator=(const ConfigValue& other);

	// Getter functions
	bool asBool() const;
	unsigned int asUint() const;
	std::string asString() const;
	std::pair<std::string, LogLevels> asLog() const;

	// Check the type
	ValueType getType() const;
};

#endif	// CONFIGVALUE_HPP
