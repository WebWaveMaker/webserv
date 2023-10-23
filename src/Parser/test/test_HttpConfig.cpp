#include "HttpConfig.hpp"
#include <cassert>
#include <iostream>
#include "Parser.h"
// -std=c++98
int main() {
	std::cout << "Testing constructors" << std::endl;
	//SENDFILE
	HttpConfig hc;
	std::cerr << "hc.getDirectives(Directives::SENDFILE) = " << hc.getDirectives(Directives::SENDFILE).asBool()
			  << std::endl;
	assert(hc.getDirectives(Directives::SENDFILE).asBool() == DEF_SENDFILE);
	std::cerr << "hc.setDirectives(Directives::SENDFILE, {\"off\"})" << std::endl;
	std::vector<std::string> v;
	v.push_back("off");
	hc.setDirectives("sendfile", v);
	std::cerr << "hc.getDirectives(Directives::SENDFILE) = " << hc.getDirectives(Directives::SENDFILE).asBool()
			  << std::endl;
	assert(hc.getDirectives(Directives::SENDFILE).asBool() == false);
	//KEEPALIVE_TIMEOUT
	std::cerr << "hc.getDirectives(Directives::KEEPALIVE_TIMEOUT) = "
			  << hc.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	assert(hc.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() == DEF_KEEPALIVE_TIMEOUT);
	std::cerr << "hc.setDirectives(Directives::KEEPALIVE_TIMEOUT, {\"42\"})" << std::endl;
	std::vector<std::string> v2;
	v2.push_back("42");
	hc.setDirectives("keepalive_timeout", v2);
	std::cerr << "hc.getDirectives(Directives::KEEPALIVE_TIMEOUT) = "
			  << hc.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() << std::endl;
	assert(hc.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() == 42);

	std::cerr << "hc.getDirectives(Directives::DEFAULT_TYPE) = "
			  << hc.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	assert(hc.getDirectives(Directives::DEFAULT_TYPE).asString() == DEF_DEFAULT_TYPE);
	std::cerr << "hc.setDirectives(Directives::DEFAULT_TYPE, {\"text/html\"})" << std::endl;
	std::vector<std::string> v3;
	v3.push_back("text/html");
	hc.setDirectives("default_type", v3);
	std::cerr << "hc.getDirectives(Directives::DEFAULT_TYPE) = "
			  << hc.getDirectives(Directives::DEFAULT_TYPE).asString() << std::endl;
	assert(hc.getDirectives(Directives::DEFAULT_TYPE).asString() == "text/html");

	std::cerr << "hc.getDirectives(Directives::ERROR_LOG) = " << hc.getDirectives(Directives::ERROR_LOG).asLog().first
			  << ", " << hc.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	assert(hc.getDirectives(Directives::ERROR_LOG).asLog().first == DEF_ERROR_LOG_PATH);
	assert(hc.getDirectives(Directives::ERROR_LOG).asLog().second == LogLevels::LOG_ERROR);
	std::cerr << "hc.setDirectives(Directives::ERROR_LOG, {\"logs/error.log\", \"debug\"})" << std::endl;
	std::vector<std::string> v4;
	v4.push_back("logs/error.log");
	v4.push_back("debug");
	hc.setDirectives("error_log", v4);
	std::cerr << "hc.getDirectives(Directives::ERROR_LOG) = " << hc.getDirectives(Directives::ERROR_LOG).asLog().first
			  << ", " << hc.getDirectives(Directives::ERROR_LOG).asLog().second << std::endl;
	assert(hc.getDirectives(Directives::ERROR_LOG).asLog().first == "logs/error.log");
	assert(hc.getDirectives(Directives::ERROR_LOG).asLog().second == LogLevels::LOG_DEBUG);

	std::cerr << "hc.getDirectives(Directives::CLIENT_MAX_BODY_SIZE) = "
			  << hc.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	assert(hc.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() == DEF_CLIENT_MAX_BODY_SIZE);
	std::cerr << "hc.setDirectives(Directives::CLIENT_MAX_BODY_SIZE, {\"42\"})" << std::endl;
	std::vector<std::string> v5;
	v5.push_back("42");
	hc.setDirectives("client_max_body_size", v5);
	std::cerr << "hc.getDirectives(Directives::CLIENT_MAX_BODY_SIZE) = "
			  << hc.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() << std::endl;
	assert(hc.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() == 42);

	std::cerr << "hc.getDirectives(Directives::LISTEN) = " << hc.getDirectives(Directives::LISTEN).asUint()
			  << std::endl;
	assert(hc.getDirectives(Directives::LISTEN).asUint() == DEF_LISTEN);
	std::cerr << "hc.setDirectives(Directives::LISTEN, {\"42\"})" << std::endl;
	// std::vector<std::string> v6;
	// v6.push_back("42");
	// hc.setDirectives("listen", v6);
	// std::cerr << "hc.getDirectives(Directives::LISTEN) = " << hc.getDirectives(Directives::LISTEN).asUint()
	// 		  << std::endl;
	// assert(hc.getDirectives(Directives::LISTEN).asUint() == 42);

	std::cerr << "hc.getDirectives(Directives::SERVER_NAME) = " << hc.getDirectives(Directives::SERVER_NAME).asString()
			  << std::endl;
	assert(hc.getDirectives(Directives::SERVER_NAME).asString() == DEF_SERVER_NAME);
	std::cerr << "hc.setDirectives(Directives::SERVER_NAME, {\"localhost\"})" << std::endl;
	// std::vector<std::string> v7;
	// v7.push_back("localhost");
	// hc.setDirectives("server_name", v7);
	// std::cerr << "hc.getDirectives(Directives::SERVER_NAME) = " << hc.getDirectives(Directives::SERVER_NAME).asString()
	// 		  << std::endl;
	// assert(hc.getDirectives(Directives::SERVER_NAME).asString() == "localhost");

	std::cerr << "hc.getDirectives(Directives::ROOT) = " << hc.getDirectives(Directives::ROOT).asString() << std::endl;
	assert(hc.getDirectives(Directives::ROOT).asString() == DEF_ROOT);
	std::cerr << "hc.setDirectives(Directives::ROOT, {\"/var/www/html\"})" << std::endl;
	// std::vector<std::string> v8;
	// hc.setDirectives("root", v8);
	// std::cerr << "hc.getDirectives(Directives::ROOT) = " << hc.getDirectives(Directives::ROOT).asString() << std::endl;
	// assert(hc.getDirectives(Directives::ROOT).asString() == "/var/www/html");

	std::cerr << "hc.getDirectives(Directives::AUTOINDEX) = " << hc.getDirectives(Directives::AUTOINDEX).asBool()
			  << std::endl;
	assert(hc.getDirectives(Directives::AUTOINDEX).asBool() == DEF_AUTOINDEX);
	std::cerr << "hc.setDirectives(Directives::AUTOINDEX, {\"on\"})" << std::endl;
	// std::vector<std::string> v9;
	// v9.push_back("on");
	// hc.setDirectives("autoindex", v9);
	// std::cerr << "hc.getDirectives(Directives::AUTOINDEX) = " << hc.getDirectives(Directives::AUTOINDEX).asBool()
	// 		  << std::endl;
	// assert(hc.getDirectives(Directives::AUTOINDEX).asBool() == true);

	std::cerr << "hc.getDirectives(Directives::INDEX) = " << hc.getDirectives(Directives::INDEX).asStrVec()[0]
			  << std::endl;
	assert(hc.getDirectives(Directives::INDEX).asStrVec()[0] == DEF_INDEX);
	std::cerr << "hc.setDirectives(Directives::INDEX, {\"index.php\"})" << std::endl;
	// std::vector<std::string> v10;
	// v10.push_back("index.php");
	// hc.setDirectives("index", v10);
	// std::cerr << "hc.getDirectives(Directives::INDEX) = " << hc.getDirectives(Directives::INDEX).asString()
	// 		  << std::endl;
	// assert(hc.getDirectives(Directives::INDEX).asString() == std::string("index.php"));

	std::cerr << "hc.getDirectives(Directives::LIMIT_EXCEPT) = ";
	ConfigValue cv = hc.getDirectives(Directives::LIMIT_EXCEPT);
	for (HttpMethod m : cv.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	std::cerr << "hc.setDirectives(Directives::LIMIT_EXCEPT, {\"GET\", \"POST\", \"DELETE\", \"PUT\"})" << std::endl;
	// std::vector<std::string> v11;
	// v11.push_back("GET");
	// v11.push_back("POST");
	// v11.push_back("DELETE");
	// hc.setDirectives("limit_except", v11);
	// std::cerr << "hc.getDirectives(Directives::LIMIT_EXCEPT) = ";
	// cv = hc.getDirectives(Directives::LIMIT_EXCEPT);
	// for (HttpMethod m : cv.asMedVec()) {
	// 	std::cerr << m << ", ";
	// }
	// std::cerr << std::endl;
	// assert(cv.asMedVec() == std::vector<HttpMethod>({GET, POST, DELETE}));

	std::cerr << "hc.getDirectives(ERROR_PAGE) = ";
	ConfigValue cvEP(hc.getErrorPage(404));
	std::cerr << cvEP.asString() << std::endl;
	assert(cvEP.asString() == DEF_ERROR_PAGE);
	std::cerr << "hc.setDirectives(ERROR_PAGE, {\"404\", \"test.html\"})" << std::endl;
	std::vector<std::string> v12;
	v12.push_back("404");
	v12.push_back("test.html");
	hc.setDirectives("error_page", v12);
	std::cerr << "hc.getDirectives(ERROR_PAGE) = ";
	cvEP = hc.getErrorPage(404);
	std::cerr << cvEP.asString() << std::endl;
	assert(cvEP.asString() == "test.html");

	std::cout << std::endl;
	std::cout << "Testing copy constructor" << std::endl;
	HttpConfig hc2(hc);
	assert(hc.getDirectives(Directives::SENDFILE).asBool() == hc2.getDirectives(Directives::SENDFILE).asBool());
	assert(hc.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint() ==
		   hc2.getDirectives(Directives::KEEPALIVE_TIMEOUT).asUint());
	assert(hc.getDirectives(Directives::DEFAULT_TYPE).asString() ==
		   hc2.getDirectives(Directives::DEFAULT_TYPE).asString());
	assert(hc.getDirectives(Directives::ERROR_LOG).asLog().first ==
		   hc2.getDirectives(Directives::ERROR_LOG).asLog().first);
	assert(hc.getDirectives(Directives::ERROR_LOG).asLog().second ==
		   hc2.getDirectives(Directives::ERROR_LOG).asLog().second);
	assert(hc.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint() ==
		   hc2.getDirectives(Directives::CLIENT_MAX_BODY_SIZE).asUint());
	assert(hc.getDirectives(Directives::LISTEN).asUint() == hc2.getDirectives(Directives::LISTEN).asUint());
	assert(hc.getDirectives(Directives::SERVER_NAME).asString() ==
		   hc2.getDirectives(Directives::SERVER_NAME).asString());
	assert(hc.getDirectives(Directives::ROOT).asString() == hc2.getDirectives(Directives::ROOT).asString());
	assert(hc.getDirectives(Directives::AUTOINDEX).asBool() == hc2.getDirectives(Directives::AUTOINDEX).asBool());
	assert(hc.getDirectives(Directives::INDEX).asStrVec()[0] == hc2.getDirectives(Directives::INDEX).asStrVec()[0]);
	assert(hc.getDirectives(Directives::LIMIT_EXCEPT).asMedVec() ==
		   hc2.getDirectives(Directives::LIMIT_EXCEPT).asMedVec());
	assert(hc.getErrorPage(404) == hc2.getErrorPage(404));
	std::cout << std::endl;

	std::cout << "Testing map" << std::endl;
	std::map<int, HttpConfig> m;
	HttpConfig hc3;
	HttpConfig hc4;
	std::vector<std::string> v13;
	v13.push_back("off");
	hc3.setDirectives("sendfile", v13);
	std::vector<std::string> v14;
	v14.push_back("on");
	hc4.setDirectives("sendfile", v14);
	m.insert(std::make_pair(1, hc3));
	m.insert(std::make_pair(2, hc4));
	std::cerr << "m[1].getDirectives(Directives::SENDFILE) = " << m[1].getDirectives(Directives::SENDFILE).asBool()
			  << std::endl;
	assert(m[1].getDirectives(Directives::SENDFILE).asBool() == false);
	std::cerr << "m[2].getDirectives(Directives::SENDFILE) = " << m[2].getDirectives(Directives::SENDFILE).asBool()
			  << std::endl;
	assert(m[2].getDirectives(Directives::SENDFILE).asBool() == true);
	std::cout << std::endl;

	std::cout << "Testing vector" << std::endl;
	std::vector<HttpConfig> v15;
	HttpConfig hc5;
	HttpConfig hc6;
	std::vector<std::string> v16;
	v16.push_back("off");
	hc5.setDirectives("sendfile", v16);
	std::vector<std::string> v17;
	v17.push_back("on");
	hc6.setDirectives("sendfile", v17);
	v15.push_back(hc5);
	v15.push_back(hc6);
	std::cerr << "v15[0].getDirectives(Directives::SENDFILE) = " << v15[0].getDirectives(Directives::SENDFILE).asBool()
			  << std::endl;
	assert(v15[0].getDirectives(Directives::SENDFILE).asBool() == false);
	std::cerr << "v15[1].getDirectives(Directives::SENDFILE) = " << v15[1].getDirectives(Directives::SENDFILE).asBool()
			  << std::endl;
	assert(v15[1].getDirectives(Directives::SENDFILE).asBool() == true);
	std::cout << std::endl;
}