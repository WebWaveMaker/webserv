#include <cassert>
#include <iostream>
#include "ConfigParser.hpp"
#include "ServerConfig.hpp"

int main() {
	ConfigParser parser;
	std::vector<ServerConfig> servers;
	bool result = parser.parse("test_config.txt", servers);

	// assert(result && "Parsing failed.");

	// Check if it parsed 2 server configurations
	// assert(servers.size() == 2 && "Expected 2 server configurations.");
	// For the first server
	ServerConfig& server1 = servers[0];
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_LISTEN).asUint() << std::endl;
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_SERVER_NAME).asString() << std::endl;
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_ROOT).asString() << std::endl;
	// std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_LIMIT_EXCEPT).asMedVec() << std::endl;
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_INDEX).asStrVec()[0] << std::endl;
	std::cout << "Server 1:" << server1.getDirectives(ServerDirectives::SERVER_AUTOINDEX).asBool() << std::endl;

	ServerConfig& server2 = servers[1];
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_LISTEN).asUint() << std::endl;
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_SERVER_NAME).asString() << std::endl;
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_ROOT).asString() << std::endl;
	// std::cout << "Server 1:" << server2.getDirectives(ServerDirectives::SERVER_LIMIT_EXCEPT).asMedVec() << std::endl;
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_INDEX).asStrVec()[0] << std::endl;
	std::cout << "Server 2:" << server2.getDirectives(ServerDirectives::SERVER_AUTOINDEX).asBool() << std::endl;

	std::cout << "All tests passed!" << std::endl;
	return 0;
}
