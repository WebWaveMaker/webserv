#include "ServerConfig.hpp"
#include <cassert>
#include <iostream>
#include "HttpConfig.hpp"
#include "Parser.h"

// -std=c++98
int main() {
	//SENDFILE
	HttpConfig hc;
	std::cerr << hc.getDirectives(Directives::SENDFILE).asBool() << std::endl;
	std::cerr << hc.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	std::cerr << hc.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	std::cerr << hc.getDirectives(Directives::ERROR_LOG).asLog().first << ", "
			  << hc.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	std::cerr << hc.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	std::cerr << hc.getDirectives(Directives::LISTEN).asUint() << std::endl;
	std::cerr << hc.getDirectives(Directives::SERVER_NAME).asString() << std::endl;
	std::cerr << hc.getDirectives(Directives::ROOT).asString() << std::endl;
	std::cerr << hc.getDirectives(Directives::AUTOINDEX).asBool() << std::endl;
	std::cerr << hc.getDirectives(Directives::INDEX).asStrVec()[0] << std::endl;
	ConfigValue cv1 = hc.getDirectives(Directives::LIMIT_EXCEPT);
	for (HttpMethod m : cv1.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	std::cerr << hc.getErrorPage(404) << std::endl;

	ServerConfig ServerC(&hc);
	std::cout << std::endl;
	std::cout << "Testing copy constructor" << std::endl;
	ServerC.setDirectives("sendfile", {"off"});
	ServerC.setDirectives("keepalive_timeout", {"42"});
	ServerC.setDirectives("default_type", {"text/html"});
	ServerC.setDirectives("error_log", {"logs/error.log", "debug"});
	ServerC.setDirectives("client_max_body_size", {"42"});
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
	ConfigValue cv2 = ServerC.getDirectives(Directives::LIMIT_EXCEPT);
	for (HttpMethod m : cv2.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	std::cerr << ServerC.getErrorPage(404) << std::endl;
	std::cerr << std::endl;

	std::cerr << "Testing copy constructor" << std::endl;

	ServerConfig ServerC2(ServerC);
	std::cerr << ServerC2.getDirectives(Directives::SENDFILE).asBool() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::ERROR_LOG).asLog().first << ", "
			  << ServerC2.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::LISTEN).asUint() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::SERVER_NAME).asString() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::ROOT).asString() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::AUTOINDEX).asBool() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::INDEX).asStrVec()[0] << std::endl;
	ConfigValue cv3 = ServerC2.getDirectives(Directives::LIMIT_EXCEPT);
	for (HttpMethod m : cv3.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	assert(ServerC2.getDirectives(Directives::SENDFILE).asBool() ==
		   ServerC.getDirectives(Directives::SENDFILE).asBool());
	assert(ServerC2.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() ==
		   ServerC.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint());
	assert(ServerC2.getDirectives(Directives::DEFAULT_TYPE).asString() ==
		   ServerC.getDirectives(Directives::DEFAULT_TYPE).asString());
	assert(ServerC2.getDirectives(Directives::ERROR_LOG).asLog().first ==
		   ServerC.getDirectives(Directives::ERROR_LOG).asLog().first);
	assert(ServerC2.getDirectives(Directives::ERROR_LOG).asLog().second ==
		   ServerC.getDirectives(Directives::ERROR_LOG).asLog().second);
	assert(ServerC2.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() ==
		   ServerC.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint());
	assert(ServerC2.getDirectives(Directives::LISTEN).asUint() == ServerC.getDirectives(Directives::LISTEN).asUint());
	assert(ServerC2.getDirectives(Directives::SERVER_NAME).asString() ==
		   ServerC.getDirectives(Directives::SERVER_NAME).asString());
	assert(ServerC2.getDirectives(Directives::ROOT).asString() == ServerC.getDirectives(Directives::ROOT).asString());
	assert(ServerC2.getDirectives(Directives::AUTOINDEX).asBool() ==
		   ServerC.getDirectives(Directives::AUTOINDEX).asBool());
	assert(ServerC2.getDirectives(Directives::INDEX).asStrVec()[0] ==
		   ServerC.getDirectives(Directives::INDEX).asStrVec()[0]);

	std::cerr << ServerC2.getDirectives(Directives::SENDFILE).asBool() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::ERROR_LOG).asLog().first << ", "
			  << ServerC2.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::LISTEN).asUint() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::SERVER_NAME).asString() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::ROOT).asString() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::AUTOINDEX).asBool() << std::endl;
	std::cerr << ServerC2.getDirectives(Directives::INDEX).asStrVec()[0] << std::endl;
	ConfigValue cv4 = ServerC2.getDirectives(Directives::LIMIT_EXCEPT);
	for (HttpMethod m : cv4.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	std::cerr << ServerC2.getErrorPage(404) << std::endl;
	std::cerr << std::endl;

	ServerConfig ServerC3;
	ServerC3 = ServerC;

	std::cerr << ServerC3.getDirectives(Directives::SENDFILE).asBool() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::ERROR_LOG).asLog().first << ", "
			  << ServerC3.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::LISTEN).asUint() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::SERVER_NAME).asString() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::ROOT).asString() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::AUTOINDEX).asBool() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::INDEX).asStrVec()[0] << std::endl;
	ConfigValue cv5 = ServerC3.getDirectives(Directives::LIMIT_EXCEPT);
	for (HttpMethod m : cv5.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	assert(ServerC3.getDirectives(Directives::SENDFILE).asBool() ==
		   ServerC.getDirectives(Directives::SENDFILE).asBool());
	assert(ServerC3.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() ==
		   ServerC.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint());
	assert(ServerC3.getDirectives(Directives::DEFAULT_TYPE).asString() ==
		   ServerC.getDirectives(Directives::DEFAULT_TYPE).asString());
	assert(ServerC3.getDirectives(Directives::ERROR_LOG).asLog().first ==
		   ServerC.getDirectives(Directives::ERROR_LOG).asLog().first);
	assert(ServerC3.getDirectives(Directives::ERROR_LOG).asLog().second ==
		   ServerC.getDirectives(Directives::ERROR_LOG).asLog().second);
	assert(ServerC3.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() ==
		   ServerC.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint());
	assert(ServerC3.getDirectives(Directives::LISTEN).asUint() == ServerC.getDirectives(Directives::LISTEN).asUint());
	assert(ServerC3.getDirectives(Directives::SERVER_NAME).asString() ==
		   ServerC.getDirectives(Directives::SERVER_NAME).asString());
	assert(ServerC3.getDirectives(Directives::ROOT).asString() == ServerC.getDirectives(Directives::ROOT).asString());
	assert(ServerC3.getDirectives(Directives::AUTOINDEX).asBool() ==
		   ServerC.getDirectives(Directives::AUTOINDEX).asBool());
	assert(ServerC3.getDirectives(Directives::INDEX).asStrVec()[0] ==
		   ServerC.getDirectives(Directives::INDEX).asStrVec()[0]);

	std::cerr << ServerC3.getDirectives(Directives::SENDFILE).asBool() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::ERROR_LOG).asLog().first << ", "
			  << ServerC3.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::LISTEN).asUint() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::SERVER_NAME).asString() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::ROOT).asString() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::AUTOINDEX).asBool() << std::endl;
	std::cerr << ServerC3.getDirectives(Directives::INDEX).asStrVec()[0] << std::endl;
	ConfigValue cv6 = ServerC3.getDirectives(Directives::LIMIT_EXCEPT);
	for (HttpMethod m : cv6.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	std::cerr << ServerC3.getErrorPage(404) << std::endl;
	std::cerr << std::endl;
}