#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : _parent(NULL) {}

ServerConfig::ServerConfig(HttpConfig* parent) : _parent(parent) {}

ServerConfig::ServerConfig(const ServerConfig& other)
	: AConfig(other), _parent(other._parent) {	// Include AConfig's copy constructor here.
	for (std::map<std::string, LocationConfig*>::const_iterator it = other._locations.begin();
		 it != other._locations.end(); ++it) {
		_locations[it->first] = new LocationConfig(*(it->second));	// Deep copy
	}
}

ServerConfig::~ServerConfig() {
	for (std::map<std::string, LocationConfig*>::iterator it = _locations.begin(); it != _locations.end(); ++it) {
		delete it->second;
	}
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
	if (this != &other) {
		AConfig::operator=(other);
		// 기존의 동적 할당된 메모리 해제
		for (std::map<std::string, LocationConfig*>::iterator it = _locations.begin(); it != _locations.end(); ++it) {
			delete it->second;
		}
		_locations.clear();

		_parent = other._parent;

		for (std::map<std::string, LocationConfig*>::const_iterator it = other._locations.begin();
			 it != other._locations.end(); ++it) {
			_locations[it->first] = new LocationConfig(*(it->second));	// 깊은 복사
		}
	}

	return *this;
}

void ServerConfig::setDirectives(const std::string& directive, const std::vector<std::string>& values) {
	if (values.empty())
		throw std::runtime_error("Invalid number of parameters for " + directive + " directive");

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
			throw std::runtime_error("Invalid number of parameters for error_log");
		_directives.insert(std::make_pair(ERROR_LOG, addLogValue(values)));
	} else if (directive == "client_max_body_size") {
		_directives.insert(std::make_pair(CLIENT_MAX_BODY_SIZE, addUnsignedIntValue(values[0])));
	} else if (directive == "listen") {
		_directives.insert(std::make_pair(LISTEN, addUnsignedIntValue(values[0])));
	} else if (directive == "server_name") {
		_directives.insert(std::make_pair(SERVER_NAME, addStringValue(values[0])));
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
				throw std::runtime_error("Invalid method");
			}
		}
		_directives.insert(std::make_pair(LIMIT_EXCEPT, ConfigValue(methods)));
	} else {
		throw std::runtime_error("Invalid directive" + directive);
	}
}

void ServerConfig::setErrorPage(const std::vector<std::string>& values) {
	const unsigned int size = values.size();
	if (size < 2) {
		throw std::runtime_error("Invalid number of parameters for error_page");
	}
	for (unsigned int i = 0; i < size - 1; i++) {
		unsigned int error_code = static_cast<unsigned int>(stringToDecimal(values[i]));
		if (error_code == 0 || error_code > 599) {
			throw std::runtime_error("Invalid error code");
		}
		_errorPages.insert(std::make_pair(error_code, values[size - 1]));
	}
}

std::string ServerConfig::getErrorPage(unsigned int error_code) const {
	std::map<unsigned int, std::string>::const_iterator it = _errorPages.find(error_code);
	if (it == _errorPages.end()) {
		return _parent->getErrorPage(error_code);
	}
	return it->second;
}

ConfigValue ServerConfig::getDirectives(Directives method) const {
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
		throw std::runtime_error("Invalid method");
	}
	std::cerr << "finded" << std::endl;
	return it->second;
}

void ServerConfig::setLocations(std::string identifier, LocationConfig* location) {
	// 이미 존재하는 경우 덮어씌우지 않음
	if (_locations.find(identifier) == _locations.end()) {
		_locations[identifier] = location;
	}
}

LocationConfig* ServerConfig::getLocation(const std::string& identifier) const {
	std::map<std::string, LocationConfig*>::const_iterator it = _locations.find(identifier);
	if (it == _locations.end()) {
		throw std::runtime_error("location not found");
	}
	return it->second;
}
