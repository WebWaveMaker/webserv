#include "CommonConfig.hpp"
#include <stdexcept>

int stringToDecimal(const std::string& value) {
	if (value.empty()) {
		throw std::runtime_error("error: empty string");
	}

	char unit = value[value.size() - 1];
	std::string numberPart = value;

	// 단위 검사
	if (!isdigit(unit)) {
		numberPart = value.substr(0, value.size() - 1);
		switch (unit) {
			case 'm':
			case 'M':
				return atoi(numberPart.c_str()) * 1000000;
			case 'k':
			case 'K':
				return atoi(numberPart.c_str()) * 1000;
			default:
				throw std::runtime_error("error: invalid unit");
		}
	}

	return atoi(value.c_str());
}

// Default constructor
CommonConfig::CommonConfig()
	: _sendFile(DEF_SENDFILE),
	  _keepAliveTimeout(DEF_KEEPALIVE_TIMEOUT),
	  _defaultType(DEF_DEFAULT_TYPE),
	  _clientMaxBodySize(DEF_CLIENT_MAX_BODY_SIZE),
	  _errorLog(std::make_pair(DEF_ERROR_LOG_PATH, DEF_ERROR_LOG_LEVEL)) {
	_errorPages[DEF_ERROR_CODE] = DEF_ERROR_PAGE;
}

// Copy constructor
CommonConfig::CommonConfig(const CommonConfig& other)
	: _sendFile(other._sendFile),
	  _keepAliveTimeout(other._keepAliveTimeout),
	  _defaultType(other._defaultType),
	  _clientMaxBodySize(other._clientMaxBodySize),
	  _errorLog(other._errorLog),
	  _errorPages(other._errorPages) {}

// Destructor
CommonConfig::~CommonConfig() {}

// Assignment operator
CommonConfig& CommonConfig::operator=(const CommonConfig& other) {
	if (this != &other) {
		_sendFile = other._sendFile;
		_keepAliveTimeout = other._keepAliveTimeout;
		_defaultType = other._defaultType;
		_clientMaxBodySize = other._clientMaxBodySize;
		_errorLog = other._errorLog;
		_errorPages = other._errorPages;
	}
	return *this;
}

bool CommonConfig::getSendFile() const {
	return _sendFile.asBool();
}

unsigned int CommonConfig::getKeepAliveTimeout() const {
	return _keepAliveTimeout.asUint();
}

std::string CommonConfig::getDefaultType() const {
	return _defaultType.asString();
}

unsigned int CommonConfig::getClientMaxBodySize() const {
	return _clientMaxBodySize.asUint();
}

std::pair<std::string, LogLevels> CommonConfig::getErrorLog() const {
	return _errorLog.asLog();
}

std::string CommonConfig::getErrorPage(unsigned int error_code) const {
	std::map<unsigned int, std::string>::const_iterator it = _errorPages.find(error_code);
	if (it == _errorPages.end()) {
		it = _errorPages.find(DEF_ERROR_CODE);
		if (it == _errorPages.end()) {
			throw std::runtime_error("Error page not found");
		}
	}
	return it->second;
}

ConfigValue CommonConfig::getValueByHttpMethod(HttpDirectives method) const {
	switch (method) {
		case HTTP_SENDFILE:
			return getSendFile();
		case HTTP_KEEPALIVE_TIMEOUT:
			return getKeepAliveTimeout();
		case HTTP_DEFAULT_TYPE:
			return getDefaultType();
		case HTTP_CLIENT_MAX_BODY_SIZE:
			return getClientMaxBodySize();
		case HTTP_ERROR_LOG:
			return getErrorLog();
		default:
			throw std::runtime_error("Unknown HttpMethod");
	}
}

void CommonConfig::setSendFile(const std::string& value) {
	_sendFile = (value == "on");
}

void CommonConfig::setKeepAliveTimeout(const std::string& value) {
	_keepAliveTimeout = static_cast<unsigned int>(std::atoi(value.c_str()));
}

void CommonConfig::setDefaultType(const std::string& value) {
	_defaultType = ConfigValue(value);
}

void CommonConfig::setClientMaxBodySize(const std::string& value) {
	_clientMaxBodySize = static_cast<unsigned int>(stringToDecimal(value));
}

void CommonConfig::setValueByHttpMethod(const std::string& directive, const std::vector<std::string>& values) {
	if (directive == "sendfile") {
		setSendFile(values[0]);
	} else if (directive == "keepalive_timeout") {
		setKeepAliveTimeout(values[0]);
	} else if (directive == "default_type") {
		setDefaultType(values[0]);
	} else if (directive == "client_max_body_size") {
		setClientMaxBodySize(values[0]);
	} else if (directive == "error_log") {
		setErrorLog(values[0], values[1]);
	} else if (directive == "error_page") {
		setErrorPage(values);
	} else {
		throw std::runtime_error("Unknown Http Directive");
	}
}

void CommonConfig::setErrorLog(const std::string& logPath, const std::string& logLevel) {
	LogLevels level;
	if (logLevel == "error") {
		level = LOG_ERROR;
	} else if (logLevel == "warn") {
		level = LOG_WARN;
	} else if (logLevel == "info") {
		level = LOG_INFO;
	} else if (logLevel == "debug") {
		level = LOG_DEBUG;
	} else {
		throw std::runtime_error("Invalid log level");
	}
	_errorLog = std::make_pair(logPath, level);
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
