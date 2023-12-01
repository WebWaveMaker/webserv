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
	throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid boolean value");
}

ConfigValue AConfig::addUnsignedIntValue(const std::string& value) {
	unsigned int unsignedValue = static_cast<unsigned int>(utils::stringToDecimal(value));
	if (unsignedValue == 0) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid unsigned integer value");
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
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid log level");
	}
	return ConfigValue(std::make_pair(value[0], level));
}

ConfigValue AConfig::addMedVecValue(const std::vector<HttpMethods>& value) {
	return ConfigValue(value);
}

ConfigValue AConfig::addStrVecValue(const std::vector<std::string>& value) {
	return ConfigValue(value);
}

bool AConfig::isDirective(Directives method) const {
	std::map<Directives, ConfigValue>::const_iterator it = _directives.find(method);
	if (it == _directives.end()) {
		return false;  // 지시어를 찾을 수 없음
	}
	return true;
}