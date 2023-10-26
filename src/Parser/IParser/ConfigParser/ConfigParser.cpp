// ConfigParser.cpp
#include "ConfigParser.hpp"
#include "HttpConfig.hpp"

ConfigParser::ConfigParser() {}

ConfigParser::~ConfigParser() {}

std::string ConfigParser::parser(const std::string& filename) {
	std::ifstream infile(filename.c_str());
	if (infile.is_open() == false) {
		throw ErrorLogger::log(__FILE__, __LINE__, __func__, "Could not open file: " + filename);
	}

	const std::string content((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
	return content;
}

bool ConfigParser::httpConfigParser(const HttpBlock& http, HttpConfig* httpConfig) {
	for (std::vector<Directive>::const_iterator it = http.directives.begin(); it != http.directives.end(); ++it) {
		httpConfig->setDirectives(it->name, it->parameters);
	}
	return true;
}

bool ConfigParser::serverConfigParser(const ServerBlock& serverBlock, ServerConfig* serverConfig) {
	for (std::vector<Directive>::const_iterator it = serverBlock.directives.begin(); it != serverBlock.directives.end();
		 ++it) {
		serverConfig->setDirectives(it->name, it->parameters);
	}

	for (std::vector<LocationBlock>::const_iterator lit = serverBlock.locations.begin();
		 lit != serverBlock.locations.end(); ++lit) {
		LocationConfig* locationConfig = new LocationConfig(serverConfig);
		if (locationConfigParser(*lit, locationConfig) == false) {
			delete locationConfig;	// Ensure memory cleanup if an error occurs.
			return false;
		}
		serverConfig->setLocations(lit->identifier, locationConfig);
	}

	return true;
}

bool ConfigParser::locationConfigParser(const LocationBlock& locationBlock, LocationConfig* locationConfig) {
	for (std::vector<Directive>::const_iterator it = locationBlock.directives.begin();
		 it != locationBlock.directives.end(); ++it) {
		locationConfig->setDirectives(it->name, it->parameters);
	}
	return true;
}

bool ConfigParser::parse(const std::string& filename, std::vector<ServerConfig*>& servers) {
	const std::string content = this->parser(filename);

	size_t position = 0;
	HttpBlock http;
	this->httpBlockTokenizer(content, position, http);
	utils::shared_ptr<HttpConfig> httpConfig(new HttpConfig());
	if (this->httpConfigParser(http, httpConfig.get()) == false) {
		return false;
	}
	for (std::vector<ServerBlock>::const_iterator it = http.servers.begin(); it != http.servers.end(); ++it) {
		ServerConfig* serverConfig = new ServerConfig(httpConfig);
		if (this->serverConfigParser(*it, serverConfig) == false) {
			delete serverConfig;  // Cleanup memory if an error occurs
			return false;
		}
		servers.push_back(serverConfig);
	}

	return true;
}

void ConfigParser::skipWhitespace(const std::string& content, size_t& position) {
	while (position < content.size() && std::isspace(content[position])) {
		position++;
	}
}

bool ConfigParser::match(const std::string& content, size_t& position, const std::string& expected) {
	this->skipWhitespace(content, position);

	const size_t expectedLength = expected.size();

	if (position + expectedLength > content.size()) {
		return false;
	}

	if (content.substr(position, expectedLength) != expected) {
		return false;
	}

	const size_t endPosition = position + expectedLength;

	if (endPosition == content.size() || std::isalnum(content[endPosition]) == false || content[endPosition] == ';') {
		position += expectedLength;
		return true;
	}

	return false;
}

bool ConfigParser::directiveTokenizer(const std::string& content, size_t& position, Directive& directive) {
	this->skipWhitespace(content, position);

	while (position < content.size() && std::isspace(content[position]) == false && content[position] != ';') {
		directive.name.push_back(content[position]);
		position++;
	}

	this->skipWhitespace(content, position);

	while (position < content.size() && content[position] != ';') {
		std::string parameter;

		while (position < content.size() && std::isspace(content[position]) == false && content[position] != ';') {
			parameter.push_back(content[position]);
			position++;
		}

		if (parameter.empty() == false) {
			directive.parameters.push_back(parameter);
		}

		this->skipWhitespace(content, position);
	}

	return this->match(content, position, ";");
}

bool ConfigParser::locationBlockTokenizer(const std::string& content, size_t& position, LocationBlock& locationBlock) {
	this->skipWhitespace(content, position);
	const size_t startPos = position;
	while (position < content.size() && content[position] != '{' && std::isspace(content[position]) == false &&
		   content[position] != ';') {
		position++;
	}
	locationBlock.identifier = content.substr(startPos, position - startPos);
	if (locationBlock.identifier.empty()) {
		locationBlock.identifier = "/";
	}
	this->skipWhitespace(content, position);
	if (this->match(content, position, "{") == false) {
		return false;
	}
	while (position < content.size()) {
		this->skipWhitespace(content, position);
		if (this->match(content, position, "}")) {
			return true;
		}
		Directive directive;
		if (this->directiveTokenizer(content, position, directive) == false) {
			return false;
		}
		locationBlock.directives.push_back(directive);
	}
	return false;
}

bool ConfigParser::serverBlockTokenizer(const std::string& content, size_t& position, ServerBlock& serverBlock) {
	if (this->match(content, position, "{") == false) {
		return false;
	}
	while (position < content.size()) {
		this->skipWhitespace(content, position);
		if (this->match(content, position, "}")) {
			return true;
		}
		if (this->match(content, position, "location")) {
			LocationBlock locationBlock;
			if (this->locationBlockTokenizer(content, position, locationBlock) == false) {
				return false;
			}
			serverBlock.locations.push_back(locationBlock);
		} else {
			Directive directive;
			if (this->directiveTokenizer(content, position, directive) == false) {
				return false;
			}
			serverBlock.directives.push_back(directive);
		}
	}
	return false;
}

bool ConfigParser::httpBlockTokenizer(const std::string& content, size_t& position, HttpBlock& httpBlock) {
	if (this->match(content, position, "http") == false) {
		throw ErrorLogger::log(__FILE__, __LINE__, __func__,
							   "Expected 'http' at position: " + std::to_string(position));
	}
	if (this->match(content, position, "{") == false) {
		throw ErrorLogger::log(__FILE__, __LINE__, __func__, "Expected '{' at position: " + std::to_string(position));
	}
	while (position < content.size()) {
		if (this->match(content, position, "}") == true) {
			return true;
		}
		if (this->match(content, position, "server")) {
			ServerBlock serverBlock;
			if (this->serverBlockTokenizer(content, position, serverBlock) == false) {
				throw ErrorLogger::log(__FILE__, __LINE__, __func__,
									   "Failed to parse server block at position: " + std::to_string(position));
			}
			httpBlock.servers.push_back(serverBlock);
		} else {
			Directive directive;
			if (this->directiveTokenizer(content, position, directive) == false) {
				throw ErrorLogger::log(__FILE__, __LINE__, __func__,
									   "Failed to parse directive at position: " + std::to_string(position));
			}
			httpBlock.directives.push_back(directive);
		}
	}
	throw ErrorLogger::log(__FILE__, __LINE__, __func__, "Expected '}' at position: " + std::to_string(position));
}
