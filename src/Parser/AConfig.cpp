#include "AConfig.hpp"

AConfig::AConfig() {}

AConfig::~AConfig() {}

AConfig::AConfig(const AConfig& other) : _directives(other._directives), _errorPages(other._errorPages) {}

AConfig& AConfig::operator=(const AConfig& other) {
	if (this != &other) {
		_directives = other._directives;
		_errorPages = other._errorPages;
	}
	return *this;
}

ConfigValue AConfig::addBooleanValue(const std::string& value) {
	if (value == "on") {
		return ConfigValue(true);
	}
	if (value == "off") {
		return ConfigValue(false);
	}
	throw std::runtime_error("Invalid boolean value");
}

ConfigValue AConfig::addUnsignedIntValue(const std::string& value) {
	unsigned int unsignedValue = static_cast<unsigned int>(stringToDecimal(value));
	if (unsignedValue == 0) {
		throw std::runtime_error("Invalid unsigned int value");
	}
	return ConfigValue((unsigned int)unsignedValue);
}

ConfigValue AConfig::addStringValue(const std::string& value) {
	return ConfigValue(value);
}

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
		throw std::runtime_error("Invalid log level");
	}
	return ConfigValue(std::make_pair(value[0], level));
}

ConfigValue AConfig::addMedVecValue(const std::vector<HttpMethod>& value) {
	return ConfigValue(value);
}

ConfigValue AConfig::addStrVecValue(const std::vector<std::string>& value) {
	return ConfigValue(value);
}
