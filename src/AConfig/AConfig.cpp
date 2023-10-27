/**
 * @file AConfig.cpp
 * @brief Definitions for the AConfig class.
 */

#include "AConfig.hpp"

/**
 * @brief Default constructor. Initializes data members.
 */
AConfig::AConfig() {}

/**
 * @brief Destructor.
 */
AConfig::~AConfig() {}

/**
 * @brief Copy constructor.
 * @param other The object to copy.
 */
AConfig::AConfig(const AConfig& other) : _directives(other._directives), _errorPages(other._errorPages) {}

/**
 * @brief Copy assignment operator.
 * @param other The object to copy.
 * @return Reference to *this.
 */
AConfig& AConfig::operator=(const AConfig& other) {
	if (this != &other) {
		_directives = other._directives;
		_errorPages = other._errorPages;
	}
	return *this;
}

/**
 * @brief Utility function to add a boolean value.
 * @param value The value to add.
 * @return ConfigValue object containing the added value.
 * @throw std::runtime_error if the value is invalid.
 */
ConfigValue AConfig::addBooleanValue(const std::string& value) {
	if (value == "on") {
		return ConfigValue(true);
	}
	if (value == "off") {
		return ConfigValue(false);
	}
	throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid boolean value");
}

/**
 * @brief Utility function to add an unsigned integer value.
 * @param value The value to add.
 * @return ConfigValue object containing the added value.
 * @throw std::runtime_error if the value is invalid.
 */
ConfigValue AConfig::addUnsignedIntValue(const std::string& value) {
	unsigned int unsignedValue = static_cast<unsigned int>(utils::stringToDecimal(value));
	if (unsignedValue == 0) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid unsigned integer value");
	}
	return ConfigValue((unsigned int)unsignedValue);
}

/**
 * @brief Utility function to add a string value.
 * @param value The value to add.
 * @return ConfigValue object containing the added value.
 */
ConfigValue AConfig::addStringValue(const std::string& value) {
	return ConfigValue(value);
}
/**
 * @brief Utility function to add a log level and its corresponding value.
 * @param value A vector containing the log level and its corresponding value.
 * @return ConfigValue object containing the added value.
 * @throw std::runtime_error if the value is invalid.
 */
ConfigValue AConfig::addLogValue(const std::vector<std::string>& value) {
	LogLevels level;
	if (value[1] == "debug") {
		level = LOG_DEBUG;
	} else if (value[1] == "info") {
		level = LOG_INFO;
	} else if (value[1] == "warn") {
		level = LOG_WARN;
	} else if (value[1] == "error") {
		level = LOG_ERROR;
	} else {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid log level");
	}
	return ConfigValue(std::make_pair(value[0], level));
}

/**
 * @brief Utility function to add a list of HTTP methods.
 * @param value A vector containing HTTP methods.
 * @return ConfigValue object containing the added value.
 */
ConfigValue AConfig::addMedVecValue(const std::vector<HttpMethods>& value) {
	return ConfigValue(value);
}

/**
 * @brief Utility function to add a list of strings.
 * @param value A vector containing strings.
 * @return ConfigValue object containing the added value.
 */
ConfigValue AConfig::addStrVecValue(const std::vector<std::string>& value) {
	return ConfigValue(value);
}
