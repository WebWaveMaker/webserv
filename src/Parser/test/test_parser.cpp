#include <cassert>
#include <iostream>
#include "ConfigParser.hpp"
#include "ServerConfig.hpp"

int main() {
	ConfigParser parser;
	std::vector<ServerConfig*> servers;
	bool result = parser.parse("test_config.txt", servers);

	ServerConfig* server = servers[0];
	{
		LocationConfig* location = server->getLocation("/");
		assert(location->getDirectives(ROOT).asString() == "/var/www/empty");
		assert(location->getDirectives(INDEX).asStrVec()[0] == DEF_INDEX);
		assert(location->getDirectives(AUTOINDEX).asBool() == DEF_AUTOINDEX);
		assert(location->getDirectives(LIMIT_EXCEPT).asMedVec().size() == 4);
	}
	return 0;
}
