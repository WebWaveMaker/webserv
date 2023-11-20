#include "LocationConfig.hpp"

LocationConfig::LocationConfig() : _parent(NULL) {}

LocationConfig::LocationConfig(utils::shared_ptr<ServerConfig> parent) : _parent(parent) {}

LocationConfig::LocationConfig(const LocationConfig& other)
	: AConfig(other), _parent(other._parent), _path(other._path) {}

LocationConfig::~LocationConfig() {}

LocationConfig& LocationConfig::operator=(const LocationConfig& other) {
	if (this != &other) {
		AConfig::operator=(other);
		_parent = other._parent;
		_path = other._path;
	}
	return *this;
}

void LocationConfig::setDirectives(const std::string& directive, const std::vector<std::string>& values) {
	if (values.empty())
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
									  "LocationConfig :Invalid number of parameters for " + directive);
	ConfigSyntax::checkSyntax(directive, values);
	if (directive == "sendfile") {
		_directives.insert(std::make_pair(SENDFILE, addBooleanValue(values[0])));
	} else if (directive == "return") {
		_directives.insert(std::make_pair(RETURN, addStringValue(values[0])));
	} else if (directive == "cgi_index") {
		_directives.insert(std::make_pair(CGI_INDEX, addStrVecValue(values)));
	} else if (directive == "error_page") {
		setErrorPage(values);
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
				throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
											  "LocationConfig :Invalid method for limit_except");
			}
		}
		_directives.insert(std::make_pair(LIMIT_EXCEPT, ConfigValue(methods)));
	} else {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "LocationConfig :Invalid directive " + directive);
	}
}

void LocationConfig::setErrorPage(const std::vector<std::string>& values) {
	const unsigned int size = values.size();
	if (size < 2) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
									  "LocationConfig :Invalid number of parameters for error_page");
	}
	for (unsigned int i = 0; i < size - 1; i++) {
		unsigned int error_code = static_cast<unsigned int>(utils::stringToDecimal(values[i]));
		if (error_code == 0 || error_code > 599) {
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
										  "LocationConfig :Invalid error code for error_page");
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
		} else if (method == RETURN) {
			return _parent->getDirectives(RETURN);
		} else if (method == CGI_INDEX) {
			return _parent->getDirectives(CGI_INDEX);
		}
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "LocationConfig :Invalid directive");
	}
	return it->second;
}

std::string LocationConfig::getOwnRoot() {
	std::string str;
	std::map<Directives, ConfigValue>::iterator it = _directives.find(ROOT);
	if (it == _directives.end()) {
		return std::string();  // 지시어를 찾을 수 없음
	}
	str = it->second.asString();  // 결과를 참조를 통해 반환
	return str;					  // 성공적으로 값을 찾음
}

std::vector<std::string> LocationConfig::getOwnIndex() {
	std::vector<std::string> vec;
	std::map<Directives, ConfigValue>::iterator it = _directives.find(INDEX);
	if (it == _directives.end()) {
		return std::vector<std::string>();	// 지시어를 찾을 수 없음
	}
	vec = it->second.asStrVec();  // 결과를 참조를 통해 반환
	return vec;					  // 성공적으로 값을 찾음
}

bool LocationConfig::getOwnConfirmedMethods(Directives method) {
	std::map<Directives, ConfigValue>::iterator it = _directives.find(method);
	if (it == _directives.end()) {
		return false;  // 지시어를 찾을 수 없음
	}
	return true;  // 성공적으로 값을 찾음
}
bool LocationConfig::isCgi() {
	std::map<Directives, ConfigValue>::const_iterator it = _directives.find(CGI_INDEX);
	if (it == _directives.end()) {
		return false;  // 지시어를 찾을 수 없음
	}
	return true;  // 성공적으로 값을 찾음
}

bool LocationConfig::isRedirect() {
	std::map<Directives, ConfigValue>::const_iterator it = _directives.find(RETURN);
	if (it == _directives.end()) {
		return false;  // 지시어를 찾을 수 없음
	}
	return true;  // 성공적으로 값을 찾음
}

utils::shared_ptr<ServerConfig> LocationConfig::getParent() {
	return _parent;
}

std::string LocationConfig::getMimeTypes(const std::string& extension) const {
	return this->_parent->getMimeTypes(extension);
}

std::string LocationConfig::getPath() const {
	return _path;
}

void LocationConfig::setPath(const std::string& path) {
	ConfigSyntax::locationSyntax(path);
	_path = path;
}