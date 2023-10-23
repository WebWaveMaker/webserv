#include <cassert>
#include <iostream>
#include "HttpConfig.hpp"
#include "LocationConfig.hpp"
#include "Parser.h"
#include "ServerConfig.hpp"

// -std=c++98
int main() {
	//SENDFILE
	HttpConfig hc;
	ServerConfig ServerC(&hc);
	ServerC.setDirectives("sendfile", {"off"});
	ServerC.setDirectives("keepalive_timeout", {"42"});
	ServerC.setDirectives("default_type", {"text/html"});
	ServerC.setDirectives("error_log", {"logs/error.log", "debug"});
	ServerC.setDirectives("client_max_body_size", {"2m"});
	ServerC.setDirectives("listen", {"42"});
	ServerC.setDirectives("server_name", {"localhost"});
	ServerC.setDirectives("root", {"/var/www/html"});
	ServerC.setDirectives("autoindex", {"on"});
	ServerC.setDirectives("index", {"index.php"});
	ServerC.setDirectives("limit_except", {"GET", "POST", "DELETE"});
	ServerC.setErrorPage({"404", "test.html"});
	std::cerr << ServerC.getDirectives(Directives::SENDFILE).asBool() << std::endl;
	std::cerr << ServerC.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	std::cerr << ServerC.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	std::cerr << ServerC.getDirectives(Directives::ERROR_LOG).asLog().first << ", "
			  << ServerC.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	std::cerr << ServerC.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	std::cerr << ServerC.getDirectives(Directives::LISTEN).asUint() << std::endl;
	std::cerr << ServerC.getDirectives(Directives::SERVER_NAME).asString() << std::endl;
	std::cerr << ServerC.getDirectives(Directives::ROOT).asString() << std::endl;
	std::cerr << ServerC.getDirectives(Directives::AUTOINDEX).asBool() << std::endl;
	std::cerr << ServerC.getDirectives(Directives::INDEX).asStrVec()[0] << std::endl;
	std::cerr << ServerC.getDirectives(Directives::LIMIT_EXCEPT).asMedVec().size() << std::endl;
	std::cerr << "Server" << std::endl;
	LocationConfig LC(&ServerC);
	LC.setDirectives("sendfile", {"on"});
	LC.setDirectives("error_page", {"404", "location_test.html"});
	LC.setDirectives("root", {"/var/www/html/location_test"});
	LC.setDirectives("autoindex", {"off"});
	LC.setDirectives("index", {"location_test.html"});
	LC.setDirectives("limit_except", {"GET", "POST"});

	std::cerr << LC.getDirectives(Directives::SENDFILE).asBool() << std::endl;
	std::cerr << LC.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	std::cerr << LC.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	std::cerr << LC.getDirectives(Directives::ERROR_LOG).asLog().first << ", "
			  << LC.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	std::cerr << LC.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	std::cerr << LC.getDirectives(Directives::LISTEN).asUint() << std::endl;
	std::cerr << LC.getDirectives(Directives::SERVER_NAME).asString() << std::endl;
	std::cerr << LC.getDirectives(Directives::ROOT).asString() << std::endl;
	std::cerr << LC.getDirectives(Directives::AUTOINDEX).asBool() << std::endl;
	std::cerr << LC.getDirectives(Directives::INDEX).asStrVec()[0] << std::endl;
	std::cerr << LC.getDirectives(Directives::LIMIT_EXCEPT).asMedVec().size() << std::endl;

	assert(LC.getDirectives(Directives::SENDFILE).asBool() == true);
	assert(LC.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() == 42);
	assert(LC.getDirectives(Directives::DEFAULT_TYPE).asString() == "text/html");
	assert(LC.getDirectives(Directives::ERROR_LOG).asLog().first == "logs/error.log");
	assert(LC.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() == 2097152);
	assert(LC.getDirectives(Directives::LISTEN).asUint() == 42);
	assert(LC.getDirectives(Directives::SERVER_NAME).asString() == "localhost");
	assert(LC.getDirectives(Directives::ROOT).asString() == "/var/www/html/location_test");
	assert(LC.getDirectives(Directives::AUTOINDEX).asBool() == false);
	assert(LC.getDirectives(Directives::INDEX).asStrVec()[0] == "location_test.html");
	assert(LC.getDirectives(Directives::LIMIT_EXCEPT).asMedVec().size() == 2);

	std::map<std::string, LocationConfig*> _locations;
	_locations["/"] = &LC;

	assert(LC.getDirectives(Directives::SENDFILE).asBool() ==
		   _locations["/"]->getDirectives(Directives::SENDFILE).asBool());
	assert(LC.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() ==
		   _locations["/"]->getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint());
	assert(LC.getDirectives(Directives::DEFAULT_TYPE).asString() ==
		   _locations["/"]->getDirectives(Directives::DEFAULT_TYPE).asString());
	assert(LC.getDirectives(Directives::ERROR_LOG).asLog().first ==
		   _locations["/"]->getDirectives(Directives::ERROR_LOG).asLog().first);
	assert(LC.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() ==
		   _locations["/"]->getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint());
	assert(LC.getDirectives(Directives::LISTEN).asUint() ==
		   _locations["/"]->getDirectives(Directives::LISTEN).asUint());
	assert(LC.getDirectives(Directives::SERVER_NAME).asString() ==
		   _locations["/"]->getDirectives(Directives::SERVER_NAME).asString());
	assert(LC.getDirectives(Directives::ROOT).asString() ==
		   _locations["/"]->getDirectives(Directives::ROOT).asString());
	assert(LC.getDirectives(Directives::AUTOINDEX).asBool() ==
		   _locations["/"]->getDirectives(Directives::AUTOINDEX).asBool());
	assert(LC.getDirectives(Directives::INDEX).asStrVec()[0] ==
		   _locations["/"]->getDirectives(Directives::INDEX).asStrVec()[0]);
	assert(LC.getDirectives(Directives::LIMIT_EXCEPT).asMedVec().size() ==
		   _locations["/"]->getDirectives(Directives::LIMIT_EXCEPT).asMedVec().size());
}