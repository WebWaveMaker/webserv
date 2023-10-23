#include "LocationConfig.hpp"

LocationConfig::LocationConfig() : _parent(NULL) {}

LocationConfig::LocationConfig(ServerConfig* parent) : _parent(parent) {}

LocationConfig::LocationConfig(const LocationConfig& other) : AConfig(other), _parent(other._parent) {}

LocationConfig::~LocationConfig() {}

LocationConfig& LocationConfig::operator=(const LocationConfig& other) {
	if (this != &other) {
		AConfig::operator=(other);
		_parent = other._parent;
	}
	return *this;
}

void LocationConfig::setDirectives(const std::string& directive, const std::vector<std::string>& values) {
	if (values.empty())
		throw ErrorLogger::log(__FILE__, __LINE__, __func__,
							   "LocationConfig :Invalid number of parameters for " + directive);

	if (directive == "sendfile") {
		_directives.insert(std::make_pair(SENDFILE, addBooleanValue(values[0])));
	} else if (directive == "error_page") {
		setErrorPage(values);
	} else if (directive == "root") {
		_directives.insert(std::make_pair(ROOT, addStringValue(values[0])));
	} else if (directive == "autoindex") {
		_directives.insert(std::make_pair(AUTOINDEX, addBooleanValue(values[0])));
	} else if (directive == "index") {
		_directives.insert(std::make_pair(INDEX, addStrVecValue(values)));
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
				throw ErrorLogger::log(__FILE__, __LINE__, __func__, "LocationConfig :Invalid method for limit_except");
			}
		}
		_directives.insert(std::make_pair(LIMIT_EXCEPT, ConfigValue(methods)));
	} else {
		throw ErrorLogger::log(__FILE__, __LINE__, __func__, "LocationConfig :Invalid directive " + directive);
	}
}

void LocationConfig::setErrorPage(const std::vector<std::string>& values) {
	const unsigned int size = values.size();
	if (size < 2) {
		throw ErrorLogger::log(__FILE__, __LINE__, __func__,
							   "LocationConfig :Invalid number of parameters for error_page");
	}
	for (unsigned int i = 0; i < size - 1; i++) {
		unsigned int error_code = static_cast<unsigned int>(stringToDecimal(values[i]));
		if (error_code == 0 || error_code > 599) {
			throw ErrorLogger::log(__FILE__, __LINE__, __func__, "LocationConfig :Invalid error code for error_page");
		}
		_errorPages.insert(std::make_pair(error_code, values[size - 1]));
	}
}

std::string LocationConfig::getErrorPage(unsigned int error_code) const {
	std::map<unsigned int, std::string>::const_iterator it = _errorPages.find(error_code);
	if (it == _errorPages.end()) {
		return _parent->getErrorPage(error_code);
	}
	return it->second;
}

ConfigValue LocationConfig::getDirectives(Directives method) const {
	std::map<Directives, ConfigValue>::const_iterator it = _directives.find(method);
	if (it == _directives.end()) {
		if (method == SENDFILE) {
			return _parent->getDirectives(SENDFILE);
		} else if (method == KEEPALIVE_TIMEOUT) {
			return _parent->getDirectives(KEEPALIVE_TIMEOUT);
		} else if (method == DEFAULT_TYPE) {
			return _parent->getDirectives(DEFAULT_TYPE);
		} else if (method == ERROR_LOG) {
			return _parent->getDirectives(ERROR_LOG);
		} else if (method == CLIENT_MAX_BODY_SIZE) {
			return _parent->getDirectives(CLIENT_MAX_BODY_SIZE);
		} else if (method == LIMIT_EXCEPT) {
			return _parent->getDirectives(LIMIT_EXCEPT);
		} else if (method == LISTEN) {
			return _parent->getDirectives(LISTEN);
		} else if (method == SERVER_NAME) {
			return _parent->getDirectives(SERVER_NAME);
		} else if (method == ROOT) {
			return _parent->getDirectives(ROOT);
		} else if (method == AUTOINDEX) {
			return _parent->getDirectives(AUTOINDEX);
		} else if (method == INDEX) {
			return _parent->getDirectives(INDEX);
		}
		throw ErrorLogger::log(__FILE__, __LINE__, __func__, "LocationConfig :Invalid directive");
	}
	return it->second;
}