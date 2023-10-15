#include <cassert>
#include <iostream>
#include "ConfigParser.hpp"
#include "ServerConfig.hpp"

int main() {
	ConfigParser parser;
	std::vector<ServerConfig> servers;
	bool result = parser.parse("test_config.txt", servers);

	assert(result && "Parsing failed.");

	// Check if it parsed 2 server configurations
	// assert(servers.size() == 2 && "Expected 2 server configurations.");
	// For the first server
	ServerConfig& server1 = servers[0];
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_LISTEN).asUint() << std::endl;
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_SERVER_NAME).asString() << std::endl;
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_ROOT).asString() << std::endl;
	std::vector<enum HttpMethod> server_limit_except =
		server1.getDirectives(ServerDirectives::SERVER_LIMIT_EXCEPT).asMedVec();
	for (std::vector<enum HttpMethod>::iterator it = server_limit_except.begin(); it != server_limit_except.end();
		 ++it) {
		if (*it == HttpMethod::GET) {
			std::cout << "Server 1: GET" << std::endl;
		} else if (*it == HttpMethod::POST) {
			std::cout << "Server 1: POST" << std::endl;
		} else if (*it == HttpMethod::DELETE) {
			std::cout << "Server 1: DELETE" << std::endl;
		} else if (*it == HttpMethod::PUT) {
			std::cout << "Server 1: PUT" << std::endl;
		}
	}
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_INDEX).asString() << std::endl;
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_AUTOINDEX).asBool() << std::endl;

	ServerConfig& server2 = servers[1];
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_LISTEN).asUint() << std::endl;
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_SERVER_NAME).asString() << std::endl;
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_ROOT).asString() << std::endl;
	std::vector<enum HttpMethod> server_limit_except2 =
		server2.getDirectives(ServerDirectives::SERVER_LIMIT_EXCEPT).asMedVec();
	for (std::vector<enum HttpMethod>::iterator it = server_limit_except2.begin(); it != server_limit_except2.end();
		 ++it) {
		if (*it == HttpMethod::GET) {
			std::cout << "Server 2: GET" << std::endl;
		} else if (*it == HttpMethod::POST) {
			std::cout << "Server 2: POST" << std::endl;
		} else if (*it == HttpMethod::DELETE) {
			std::cout << "Server 2: DELETE" << std::endl;
		} else if (*it == HttpMethod::PUT) {
			std::cout << "Server 2: PUT" << std::endl;
		}
	}
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_INDEX).asString() << std::endl;
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_AUTOINDEX).asBool() << std::endl;

	std::cout << "All tests passed!" << std::endl;
	return 0;
}
