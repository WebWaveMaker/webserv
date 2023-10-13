#include "ServerConfig.hpp"

ServerConfig::ServerConfig(CommonConfig& commonConfig) : _commonConfig(commonConfig) {}

ServerConfig::ServerConfig(const ServerConfig& other) : _commonConfig(other._commonConfig) {}

ServerConfig::~ServerConfig() {}

ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
	if (this != &other) {
		_commonConfig = other._commonConfig;
	}
	return *this;
}

std::string ServerConfig::getErrorPage(unsigned int error_code) const {
	std::map<unsigned int, std::string>::const_iterator it = _errorPages.find(error_code);
	if (it == _errorPages.end()) {
		return _commonConfig.getErrorPage(error_code);
	}
	return it->second;
}

ConfigValue ServerConfig::getDirectives(ServerDirectives method) const {
	std::map<ServerDirectives, ConfigValue>::const_iterator it = _values.find(method);
	if (it == _values.end()) {
		if (method == SERVER_SENDFILE) {
			return _commonConfig.getDirectives(HTTP_SENDFILE);
		} else if (method == SERVER_KEEPALIVE_TIMEOUT) {
			return _commonConfig.getDirectives(HTTP_KEEPALIVE_TIMEOUT);
		} else if (method == SERVER_DEFAULT_TYPE) {
			return _commonConfig.getDirectives(HTTP_DEFAULT_TYPE);
		} else if (method == SERVER_ERROR_LOG) {
			return _commonConfig.getDirectives(HTTP_ERROR_LOG);
		} else if (method == SERVER_CLIENT_MAX_BODY_SIZE) {
			return _commonConfig.getDirectives(HTTP_CLIENT_MAX_BODY_SIZE);
		} else if (method == SERVER_LISTEN) {
			return ConfigValue(DEF_LISTEN);
		} else if (method == SERVER_SERVER_NAME) {
			return ConfigValue(DEF_SERVER_NAME);
		} else if (method == SERVER_ROOT) {
			return ConfigValue(DEF_ROOT);
		} else if (method == SERVER_AUTOINDEX) {
			return ConfigValue(DEF_AUTOINDEX);
		} else if (method == SERVER_INDEX) {
			return ConfigValue(DEF_INDEX);
		} else if (method == SERVER_LIMIT_EXCEPT) {
			return ConfigValue(DEF_LIMIT_EXCEPT);
		} else {
			throw std::runtime_error("Value not found");
		}
	}
	return it->second;
}

void ServerConfig::setDirectives(const std::string& directive, const std::vector<std::string>& values) {
	if (directive == "sendfile") {
		_values.insert(std::make_pair(SERVER_SENDFILE, addBooleanValue(values[0])));
	} else if (directive == "keepalive_timeout") {
		_values.insert(std::make_pair(SERVER_KEEPALIVE_TIMEOUT, addUnsignedIntValue(values[0])));
	} else if (directive == "error_page") {
		setErrorPage(values);
	} else if (directive == "default_type") {
		_values.insert(std::make_pair(SERVER_DEFAULT_TYPE, addStringValue(values[0])));
	} else if (directive == "error_log") {
		_values.insert(std::make_pair(SERVER_ERROR_LOG, addLogValue(values)));
	} else if (directive == "client_max_body_size") {
		_values.insert(std::make_pair(SERVER_CLIENT_MAX_BODY_SIZE, addUnsignedIntValue(values[0])));
	} else if (directive == "listen") {
		_values.insert(std::make_pair(SERVER_LISTEN, addUnsignedIntValue(values[0])));
	} else if (directive == "server_name") {
		_values.insert(std::make_pair(SERVER_SERVER_NAME, addStringValue(values[0])));
	} else if (directive == "root") {
		_values.insert(std::make_pair(SERVER_ROOT, addStringValue(values[0])));
	} else if (directive == "autoindex") {
		_values.insert(std::make_pair(SERVER_AUTOINDEX, addBooleanValue(values[0])));
	} else if (directive == "index") {
		_values.insert(std::make_pair(SERVER_INDEX, addStrVecValue(values)));
	} else if (directive == "limit_except") {
		std::vector<HttpMethod> methods;
		for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it) {
			if (*it == "GET") {
				methods.push_back(GET);
			} else if (*it == "POST") {
				methods.push_back(POST);
			} else if (*it == "DELETE") {
				methods.push_back(DELETE);
			} else if (*it == "PUT") {
				methods.push_back(PUT);
			} else {
				throw std::runtime_error("Invalid method");
			}
		}
		_values.insert(std::make_pair(SERVER_LIMIT_EXCEPT, ConfigValue(methods)));
	} else {
		throw std::runtime_error("Invalid directive" + directive);
	}
}

void ServerConfig::setErrorPage(const std::vector<std::string>& values) {
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
