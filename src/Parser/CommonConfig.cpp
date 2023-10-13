#include "CommonConfig.hpp"

// Default constructor
CommonConfig::CommonConfig() : _values(), _errorPages() {}

// Copy constructor
CommonConfig::CommonConfig(const CommonConfig& other) : _values(other._values), _errorPages(other._errorPages) {}

// Destructor
CommonConfig::~CommonConfig() {}

// Assignment operator
CommonConfig& CommonConfig::operator=(const CommonConfig& other) {
	if (this != &other) {
		_values = other._values;
		_errorPages = other._errorPages;
	}
	return *this;
}

std::string CommonConfig::getErrorPage(unsigned int error_code) const {
	std::map<unsigned int, std::string>::const_iterator it = _errorPages.find(error_code);
	if (it == _errorPages.end()) {
		return DEF_ERROR_PAGE;
	}
	return it->second;
}

ConfigValue CommonConfig::getDirectives(HttpDirectives method) const {
	std::map<HttpDirectives, ConfigValue>::const_iterator it = _values.find(method);
	if (it == _values.end()) {
		if (method == HTTP_ERROR_LOG) {
			return ConfigValue(std::make_pair(DEF_ERROR_LOG_PATH, DEF_ERROR_LOG_LEVEL));
		} else if (method == HTTP_DEFAULT_TYPE) {
			return ConfigValue(DEF_DEFAULT_TYPE);
		} else if (method == HTTP_SENDFILE) {
			return ConfigValue(true);
		} else if (method == HTTP_KEEPALIVE_TIMEOUT) {
			return ConfigValue(DEF_KEEPALIVE_TIMEOUT);
		} else if (method == HTTP_CLIENT_MAX_BODY_SIZE) {
			return ConfigValue(DEF_CLIENT_MAX_BODY_SIZE);
		}
		throw std::runtime_error("Value not found");
	}
	return it->second;
}

void CommonConfig::setDirectives(const std::string& directive, const std::vector<std::string>& values) {
	if (directive == "sendfile") {
		_values.insert(std::make_pair(HTTP_SENDFILE, addBooleanValue(values[0])));
	} else if (directive == "keepalive_timeout") {
		_values.insert(std::make_pair(HTTP_KEEPALIVE_TIMEOUT, addUnsignedIntValue(values[0])));
	} else if (directive == "error_page") {
		setErrorPage(values);
	} else if (directive == "default_type") {
		_values.insert(std::make_pair(HTTP_DEFAULT_TYPE, addStringValue(values[0])));
	} else if (directive == "error_log") {
		_values.insert(std::make_pair(HTTP_ERROR_LOG, addLogValue(values)));
	} else if (directive == "client_max_body_size") {
		_values.insert(std::make_pair(HTTP_CLIENT_MAX_BODY_SIZE, addUnsignedIntValue(values[0])));
	} else {
		throw std::runtime_error("Invalid directive");
	}
}

void CommonConfig::setErrorPage(const std::vector<std::string>& values) {
	//error page is the last value
	std::string errorPage;
	if (values.size() == 1) {
		errorPage = values[0];
	} else {
		errorPage = values[values.size() - 1];
	}

	for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end() - 1; ++it) {
		unsigned int errorCode = static_cast<unsigned int>(std::atoi(it->c_str()));
		if (errorCode <= 0) {
			throw std::runtime_error("Invalid error code");
		}
		_errorPages[errorCode] = errorPage;
	}
}
