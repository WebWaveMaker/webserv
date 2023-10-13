#include <iostream>
#include "ConfigParser.hpp"

int main() {
	ConfigParser parser;
	CommonConfig config = CommonConfig();
	std::string filename = "test_config.txt";  // Name of our sample configuration file

	try {
		if (parser.parse(filename, config)) {
			std::cout << "HTTP_SENDFILE : " << config.getDirectives(HTTP_SENDFILE).asBool() << std::endl;
			std::cout << "HTTP_KEEPALIVE_TIMEOUT : " << config.getDirectives(HTTP_KEEPALIVE_TIMEOUT).asUint()
					  << std::endl;
			std::cout << "HTTP_KEEPALIVE_TIMEOUT : " << config.getDirectives(HTTP_DEFAULT_TYPE).asString() << std::endl;
			std::cout << "HTTP_ERROR_LOG path : " << config.getDirectives(HTTP_ERROR_LOG).asLog().first << std::endl;
			std::cout << "HTTP_ERROR_LOG level : " << config.getDirectives(HTTP_ERROR_LOG).asLog().second << std::endl;
			std::cout << "HTTP_ERROR_PAGE : " << config.getErrorPage(555) << std::endl;
		}
	} catch (const std::string& e) {
		std::cerr << "Error: " << e << std::endl;
	} catch (const char* e) {
		std::cerr << "Error: " << e << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
