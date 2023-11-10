#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(utils::shared_ptr<HttpConfig> parent) : _parent(parent) {}

ServerConfig::ServerConfig(const ServerConfig& other) : AConfig(other), _parent(other._parent) {
	for (std::map<std::string, utils::shared_ptr<LocationConfig> >::const_iterator it = other._locations.begin();
		 it != other._locations.end(); ++it) {
		_locations[it->first] = utils::shared_ptr<LocationConfig>(new LocationConfig(*(it->second)));
	}
}

ServerConfig::~ServerConfig() {}

ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
	if (this != &other) {
		AConfig::operator=(other);
		_locations.clear();
		_parent = other._parent;
		for (std::map<std::string, utils::shared_ptr<LocationConfig> >::const_iterator it = other._locations.begin();
			 it != other._locations.end(); ++it) {
			_locations[it->first] = utils::shared_ptr<LocationConfig>(new LocationConfig(*(it->second)));
		}
	}
	return *this;
}

void ServerConfig::setDirectives(const std::string& directive, const std::vector<std::string>& values) {
	if (values.empty())
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid number of parameters for " + directive);

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
		std::vector<HttpMethods> methods;
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
				throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid method for limit_except");
			}
		}
		_directives.insert(std::make_pair(LIMIT_EXCEPT, ConfigValue(methods)));
	} else {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid directive " + directive);
	}
}

void ServerConfig::setErrorPage(const std::vector<std::string>& values) {
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

std::string ServerConfig::getErrorPage(unsigned int error_code) const {
	std::map<unsigned int, std::string>::const_iterator it = _errorPages.find(error_code);
	if (it == _errorPages.end()) {
		if (_parent.get() == u::nullptr_t)
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid error code");
		return _parent->getErrorPage(error_code);
	}
	return it->second;
}

ConfigValue ServerConfig::getDirectives(Directives method) const {
	std::map<Directives, ConfigValue>::const_iterator it = _directives.find(method);
	if (it == _directives.end()) {
		if (_parent.get() == u::nullptr_t)
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid error code");
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
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid directive");
	}
	std::cerr << "finded" << std::endl;
	return it->second;
}

void ServerConfig::setLocations(std::string identifier, utils::shared_ptr<LocationConfig> location) {
	// 이미 존재하는 경우 덮어씌우지 않음
	if (_locations.find(identifier) != _locations.end())
		return;
	_locations[identifier] = location;
}

utils::shared_ptr<LocationConfig> ServerConfig::getLocation(const std::string& identifier) {
	std::map<std::string, utils::shared_ptr<LocationConfig> >::const_iterator it = _locations.find(identifier);
	if (it == _locations.end()) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid location identifier");
	}
	return it->second;
}

bool ServerConfig::getOwnRoot(std::string& str) {
	std::map<Directives, ConfigValue>::iterator it = _directives.find(ROOT);
	if (it == _directives.end()) {
		return false;  // 지시어를 찾을 수 없음
	}
	str = it->second.asString();  // 결과를 참조를 통해 반환
	return true;				  // 성공적으로 값을 찾음
}

bool ServerConfig::getOwnIndex(std::vector<std::string>& vec) {
	std::map<Directives, ConfigValue>::iterator it = _directives.find(INDEX);
	if (it == _directives.end()) {
		return false;  // 지시어를 찾을 수 없음
	}
	vec = it->second.asStrVec();  // 결과를 참조를 통해 반환
	return true;				  // 성공적으로 값을 찾음
}

utils::shared_ptr<LocationConfig> ServerConfig::getLocationConfig(const std::string& reqPath) {
	// Initialize the best match as empty and its length as 0
	utils::shared_ptr<LocationConfig> bestMatch;
	std::size_t bestMatchLength = 0;

	// Iterate through the map to find the best match
	for (std::map<std::string, utils::shared_ptr<LocationConfig> >::iterator it = _locations.begin();
		 it != _locations.end(); ++it) {
		// Check if the current path is a prefix of the request path
		const std::string& currentPath = it->first;
		if (reqPath.compare(0, currentPath.length(), currentPath) == 0) {
			// Check if it's a better match than the current best match
			if (currentPath.length() > bestMatchLength) {
				bestMatch = it->second;
				bestMatchLength = currentPath.length();
			}
		}
	}

	// If we have found a matching location configuration
	// If we have found a matching location configuration
	if (bestMatch.get() != u::nullptr_t) {
		return bestMatch;
	} else {
		// No direct match found, fall back to root if it exists
		std::string root;
		if (getOwnRoot(root)) {
			// Create a new LocationConfig with the ServerConfig (this) as the parent
			utils::shared_ptr<ServerConfig> thisPtr(this);	// Create a shared_ptr to 'this'
			utils::shared_ptr<LocationConfig> rootLocation(new LocationConfig(thisPtr));

			std::vector<std::string> index;
			if (getOwnIndex(index)) {
				rootLocation->setDirectives("root", std::vector<std::string>(1, root));
				rootLocation->setDirectives("index", index);
			}
			// No need to set _parent here, as it's already set by the constructor
			return rootLocation;
		} else {
			// Handle the case where there is no root defined
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
										  "No matching location found and no root is set.");
		}
	}
}
