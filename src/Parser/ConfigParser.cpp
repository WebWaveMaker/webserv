// ConfigParser.cpp
#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {}

ConfigParser::~ConfigParser() {}

bool ConfigParser::parse(const std::string& filename, std::vector<ServerConfig>& servers) {
	std::ifstream infile(filename.c_str());
	if (infile.is_open() == false) {
		throw "Could not open file: " + filename;
	}

	std::string content((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));

	size_t position = 0;
	HttpBlock http = HttpBlock();
	bool t = this->httpBlockTokenizer(content, position, http);
	CommonConfig commonConfig;
	for (const Directive& directive : http.directives) {
		try {
			commonConfig.setDirectives(directive.name, directive.parameters);
		} catch (const std::runtime_error& e) {
			std::cerr << "Exception while setting value in commonConfig: " << e.what() << std::endl;
			return false;
		}
	}

	for (const ServerBlock& serverBlock : http.servers) {
		ServerConfig serverConfig(commonConfig);
		for (const Directive& directive : serverBlock.directives) {
			try {
				serverConfig.setDirectives(directive.name, directive.parameters);
			} catch (const std::runtime_error& e) {
				std::cerr << "Exception while setting value in serverConfig: " << e.what() << std::endl;
				return false;
			}
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

	size_t expectedLength = expected.size();
	if (position + expectedLength <= content.size() && content.substr(position, expectedLength) == expected) {
		size_t endPosition = position + expectedLength;
		if (endPosition == content.size() || std::isalnum(content[endPosition]) == false ||
			content[endPosition] == ';') {
			position += expectedLength;
			return true;
		}
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
		directive.parameters.push_back(parameter);
		this->skipWhitespace(content, position);
	}

	return this->match(content, position, ";");
}

// bool ConfigParser::locationBlockTokenizer(const std::string& content, size_t& position, LocationBlock& locationBlock) {
// 	// locationBlockTokenizer는 location 블록을 파싱합니다.
// 	this->skipWhitespace(content, position);
// 	size_t startPos = position;
// 	while (position < content.size() && content[position] != '{')
// 		position++;

// 	locationBlock.identifier = content.substr(startPos, position - startPos);
// 	this->skipWhitespace(content, position);  // 이 부분을 추가하여 공백을 건너뜁니다.

// 	if (this->match(content, position, "{") == false)
// 		return false;

// 	while (position < content.size()) {
// 		this->skipWhitespace(content, position);
// 		if (this->match(content, position, "}")) {
// 			// 종료 중괄호를 찾으면 루프를 종료합니다.
// 			return true;
// 		} else {
// 			Directive directive;
// 			if (!this->directiveTokenizer(content, position, directive))
// 				return false;
// 			locationBlock.directives.push_back(directive);
// 		}
// 	}

// 	return false;  // 블록의 종료를 찾지 못하면 false를 반환합니다.
// }

bool ConfigParser::serverBlockTokenizer(const std::string& content, size_t& position, ServerBlock& serverBlock) {
	if (this->match(content, position, "{") == false)
		return false;

	while (position < content.size()) {
		this->skipWhitespace(content, position);
		if (this->match(content, position, "}")) {
			// 종료 중괄호를 찾으면 루프를 종료합니다.
			return true;
			// } else if (this->match(content, position, "location")) {
			// 	LocationBlock locationBlock;
			// 	if (this->locationBlockTokenizer(content, position, locationBlock) == false)
			// 		return false;
			// 	serverBlock.locations.push_back(locationBlock);
		} else {
			Directive directive;
			if (this->directiveTokenizer(content, position, directive) == false)
				return false;
			serverBlock.directives.push_back(directive);
		}
	}
	return false;  // 블록의 종료를 찾지 못하면 false를 반환합니다.
}

// 	return false;  // 블록의 종료를 찾지 못하면 false를 반환합니다.
// }

bool ConfigParser::httpBlockTokenizer(const std::string& content, size_t& position, HttpBlock& httpBlock) {
	if (this->match(content, position, "http") == false) {
		throw "Expected 'http' keyword at position: " + std::to_string(position);
	}

	if (this->match(content, position, "{") == false) {
		throw "Expected '{' at position: " + std::to_string(position);
	}

	while (position < content.size() && this->match(content, position, "}") == false) {
		if (this->match(content, position, "server")) {
			ServerBlock serverBlock;
			if (this->serverBlockTokenizer(content, position, serverBlock) == false) {
				throw "Failed to parse server block inside http block at position: " + std::to_string(position);
			}
			httpBlock.servers.push_back(serverBlock);
		} else {
			Directive directive;
			if (this->directiveTokenizer(content, position, directive) == false) {
				throw "Failed to parse directive inside http block at position: " + std::to_string(position);
			}
			httpBlock.directives.push_back(directive);
		}
	}
	return true;
}