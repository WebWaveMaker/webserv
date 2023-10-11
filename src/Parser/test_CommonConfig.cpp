#include <cassert>
#include <iostream>
#include "CommonConfig.hpp"
#include "Parser.h"	 // Assuming Parser.h includes necessary classes such as CommonConfig

int main() {
	// Default Constructor Test
	CommonConfig config;

	assert(config.getValueByHttpMethod(HTTP_SENDFILE).asBool() == true);
	assert(config.getValueByHttpMethod(HTTP_KEEPALIVE_TIMEOUT).asUint() == 75);
	assert(config.getValueByHttpMethod(HTTP_DEFAULT_TYPE).asString() == "application/octet-stream");
	assert(config.getValueByHttpMethod(HTTP_ERROR_LOG).asLog() ==
		   std::make_pair(std::string("logs/error.log"), LOG_ERROR));
	assert(config.getErrorPage(500) == "error.html");
	assert(config.getValueByHttpMethod(HTTP_CLIENT_MAX_BODY_SIZE).asUint() == 1048576);
	std::cout << "GET successfully!" << std::endl;

	//set
	std::string directive = "sendfile";
	std::vector<std::string> values;
	values.push_back("false");
	config.setValueByHttpMethod(directive, values);
	assert(config.getValueByHttpMethod(HTTP_SENDFILE).asBool() == false);
	std::cout << "SET HTTP_SENDFILE successfully!" << std::endl;

	directive = "keepalive_timeout";
	values.clear();
	values.push_back("100");
	config.setValueByHttpMethod(directive, values);
	assert(config.getValueByHttpMethod(HTTP_KEEPALIVE_TIMEOUT).asUint() == 100);
	std::cout << "SET HTTP_KEEPALIVE_TIMEOUT successfully!" << std::endl;

	directive = "default_type";
	std::vector<std::string> values2;
	values2.push_back("text/html");
	config.setValueByHttpMethod(directive, values2);
	std::string result = config.getValueByHttpMethod(HTTP_DEFAULT_TYPE).asString();
	std::cout << "result: " << result << std::endl;

	std::cout << "line ----" << std::endl;
	assert(result == "text/html");
	std::cout << "SET HTTP_DEFAULT_TYPE successfully!" << std::endl;

	directive = "client_max_body_size";
	values.clear();
	values.push_back("2m");
	config.setValueByHttpMethod(directive, values);
	int result2 = config.getValueByHttpMethod(HTTP_CLIENT_MAX_BODY_SIZE).asUint();
	assert(result2 == 2097152);
	std::cout << "SET HTTP_CLIENT_MAX_BODY_SIZE successfully!" << std::endl;

	directive = "error_log";
	values.clear();
	values.push_back("logs/error.log");
	values.push_back("debug");
	config.setValueByHttpMethod(directive, values);
	assert(config.getValueByHttpMethod(HTTP_ERROR_LOG).asLog() ==
		   std::make_pair(std::string("logs/error.log"), LOG_DEBUG));
	std::cout << "SET HTTP_ERROR_LOG successfully!" << std::endl;

	directive = "error_page";
	values.clear();
	values.push_back("404");
	values.push_back("403");
	values.push_back("402");
	values.push_back("400");
	values.push_back("page_not_found.html");
	config.setValueByHttpMethod(directive, values);
	assert(config.getErrorPage(404) == "page_not_found.html");
	assert(config.getErrorPage(403) == "page_not_found.html");
	assert(config.getErrorPage(402) == "page_not_found.html");
	assert(config.getErrorPage(400) == "page_not_found.html");
	std::cout << "SET HTTP_ERROR_PAGE successfully!" << std::endl;

	std::cout << "SET successfully!" << std::endl;

	return 0;
}
