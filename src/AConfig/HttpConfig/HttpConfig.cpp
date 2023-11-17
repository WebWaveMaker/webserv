#include "HttpConfig.hpp"

HttpConfig::HttpConfig() {}

HttpConfig::HttpConfig(const HttpConfig& other) : AConfig(other), _mimeTypes(other._mimeTypes) {}

HttpConfig::~HttpConfig() {}

HttpConfig& HttpConfig::operator=(const HttpConfig& other) {
	if (this != &other) {
		AConfig::operator=(other);
		_mimeTypes = other._mimeTypes;
	}
	return *this;
}

void HttpConfig::setDirectives(const std::string& directive, const std::vector<std::string>& values) {
	ConfigSyntax::checkSyntax(directive, values);
	if (directive == "sendfile") {
		_directives.insert(std::make_pair(SENDFILE, addBooleanValue(values[0])));
	} else if (directive == "keepalive_timeout") {
		_directives.insert(std::make_pair(KEEPALIVE_TIMEOUT, addUnsignedIntValue(values[0])));
	} else if (directive == "error_page") {
		setErrorPage(values);
	} else if (directive == "default_type") {
		_directives.insert(std::make_pair(DEFAULT_TYPE, addStringValue(values[0])));
	} else if (directive == "error_log") {
		if (values.size() != 2)
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid number of parameters for error_log");
		_directives.insert(std::make_pair(ERROR_LOG, addLogValue(values)));
	} else if (directive == "client_max_body_size") {
		_directives.insert(std::make_pair(CLIENT_MAX_BODY_SIZE, addUnsignedIntValue(values[0])));
	} else if (directive == "include") {
	} else {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid directive" + directive);
	}
}

void HttpConfig::setErrorPage(const std::vector<std::string>& values) {
	const unsigned int size = values.size();
	if (size < 2) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid number of parameters for error_page");
	}
	for (unsigned int i = 0; i < size - 1; i++) {
		unsigned int error_code = static_cast<unsigned int>(utils::stringToDecimal(values[i]));
		if (error_code == 0 || error_code > 599) {
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid error code");
		}
		_errorPages.insert(std::make_pair(error_code, values[size - 1]));
	}
}

std::string HttpConfig::getErrorPage(unsigned int error_code) const {
	std::map<unsigned int, std::string>::const_iterator it = _errorPages.find(error_code);
	if (it == _errorPages.end()) {
		return std::string(DEF_ERROR_PAGE);
	}
	return it->second;
}

ConfigValue HttpConfig::getDirectives(Directives method) const {
	std::map<Directives, ConfigValue>::const_iterator it = _directives.find(method);
	if (it == _directives.end()) {
		if (method == SENDFILE) {
			return ConfigValue(DEF_SENDFILE);
		} else if (method == KEEPALIVE_TIMEOUT) {
			return ConfigValue(DEF_KEEPALIVE_TIMEOUT);
		} else if (method == DEFAULT_TYPE) {
			return ConfigValue(std::string(DEF_DEFAULT_TYPE));
		} else if (method == ERROR_LOG) {
			std::pair<std::string, LogLevels> log(std::string(DEF_ERROR_LOG_PATH), LOG_ERROR);
			return ConfigValue((std::pair<std::string, LogLevels>)log);
		} else if (method == CLIENT_MAX_BODY_SIZE) {
			return ConfigValue(DEF_CLIENT_MAX_BODY_SIZE);
		} else if (method == LISTEN) {
			return ConfigValue(DEF_LISTEN);
		} else if (method == SERVER_NAME) {
			return ConfigValue(std::string(DEF_SERVER_NAME));
		} else if (method == ROOT) {
			return ConfigValue(std::string(DEF_ROOT));
		} else if (method == AUTOINDEX) {
			return ConfigValue(DEF_AUTOINDEX);
		} else if (method == INDEX) {
			std::vector<std::string> index;
			index.push_back(std::string(DEF_INDEX));
			return ConfigValue(index);
		} else if (method == LIMIT_EXCEPT) {
			std::vector<HttpMethods> methods;
			methods.push_back(GET);
			methods.push_back(POST);
			methods.push_back(DELETE);
			methods.push_back(PUT);
			return ConfigValue(methods);
		} else {
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid directive");
		}
	}
	return it->second;
}

std::string HttpConfig::getMimeTypes(const std::string& extension) const {
	return this->_mimeTypes->getMimeType(extension) == "" ? this->getDirectives(DEFAULT_TYPE).asString()
														  : this->_mimeTypes->getMimeType(extension);
}

void HttpConfig::setMimeTypes(utils::shared_ptr<Mime> mimeTypes) {
	this->_mimeTypes = mimeTypes;
}