// #define DOCTEST_CONFIG_IMPLEMENT
// #include "doctest.h"
// #include "RequestParser.hpp"

// TEST_CASE("Valid HTTP Request Parsing") {
//     RequestParser parser;

//     SUBCASE("Parsing a valid GET request") {
//         const std::string request = "GET /index.html HTTP/1.1\r\n"
//                                     "Host: www.example.com\r\n\r\n";
//         REQUIRE(parser.parseRequest(request) == true);
//         CHECK(parser.getMethod() == "GET");
//         CHECK(parser.getPath() == "/index.html");
//         CHECK(parser.getHTTPVersion() == "HTTP/1.1");
//     }

//     SUBCASE("Parsing a valid POST request with data") {
//         const std::string request = "POST /submit-form HTTP/1.1\r\n"
//                                     "Host: www.example.com\r\n"
//                                     "Content-Type: application/x-www-form-urlencoded\r\n\r\n"
//                                     "username=johndoe&password=secret";
//         REQUIRE(parser.parseRequest(request) == true);
//         CHECK(parser.getMethod() == "POST");
//         CHECK(parser.getPath() == "/submit-form");
//         CHECK(parser.getHTTPVersion() == "HTTP/1.1");
//         CHECK(parser.getBody() == "username=johndoe&password=secret");
//     }
// }

// TEST_CASE("Invalid HTTP Request Parsing") {
//     RequestParser parser;

//     SUBCASE("Parsing an invalid request without a valid HTTP version") {
//         const std::string request = "GET /index.html\r\n"
//                                     "Host: www.example.com\r\n\r\n";
//         REQUIRE(parser.parseRequest(request) == false);
//     }

//     SUBCASE("Parsing an invalid request with missing required headers") {
//         const std::string request = "POST /submit-form HTTP/1.1\r\n\r\n"
//                                     "username=johndoe&password=secret";
//         REQUIRE(parser.parseRequest(request) == false);
//     }

//     SUBCASE("Parsing an invalid request with unsupported HTTP method") {
//         const std::string request = "INVALID /path HTTP/1.1\r\n"
//                                     "Host: www.example.com\r\n\r\n";
//         REQUIRE(parser.parseRequest(request) == false);
//     }
// }

// int main(int argc, char* argv[]) {
//     doctest::Context context;
//     context.applyCommandLine(argc, argv);
//     int result = context.run();
//     if (context.shouldExit())
//         return result;
//     return (result < 0xFF ? result : 0xFF);
// }

#include "RequestParser.hpp"

void check(void) {
	system("leaks a.out");
}

int main(void) {

	// std::atexit(check);
	// std::string request =
	// 	"GET /example_page HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: MyHTTPClient/1.0\r\nAccept: "
	// 	"text/html\r\n\r\n";
	// std::string test1 = "GET /example_page";

	// std::string test1 = "GET /ex";

	// [idx] == ' ';

	// std::string test9 = "examp";
	std::string test2 =
		"GET /example_page HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: MyHTTPClient/1.0\r\nAccept: "
		"text/html\r\n\r\nGET /example_page HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: "
		"MyHTTPClient/1.0\r\nAccept: text/html\r\n\r\nGET /example_page HTTP/1.1\r\nHost: "
		"www.example.com\r\nUser-Agent: MyHTTPClient/1.0\r\nAccept: text/html\r\n\r\n";

	// // const std::string request =
	// // 	"POST /submit-form HTTP/1.1\r\n"
	// // 	"Host: www.example.com\r\n"
	// // 	"Content-Type: application/x-www-form-urlencoded\r\n\r\n"
	// // "username=johndoe&password=secret";
	// const std::string request =
	// 	"POST /submit-formHTTP/1.1"
	// 	"username=johndoe&password=secret\r\n\r\n";
	ServerConfig config;

	RequestParser parser(config);


	// std::string request =
	// 	"GET /index.html HTTP/1.1\r\n"
	// 	"Host: www.example.com\r\n"
	// 	"Content-Length: 28\r\n\r\n"
	// 	"1234567891011121314151315";

	// // parser.parse(test1);
	request_t msg1 = parser.parse(test2);



	if (msg1.get()) {
		std::cout << msg1.get()->first << std::endl;
		std::cout << msg1.get()->second.getRawStr() << std::endl;
	}

	request_t msg2 = parser.get();

	if (msg2.get()) {
		std::cout << msg2.get()->first << std::endl;
		std::cout << msg2.get()->second.getRawStr() << std::endl;
	}
	request_t msg3 = parser.get();

	if (msg3.get()) {
		std::cout << msg3.get()->first << std::endl;
		std::cout << msg3.get()->second.getRawStr() << std::endl;
	}

	request_t msg4 = parser.get();

	if (msg4.get()) {
		std::cout << msg4.get()->first << std::endl;
		std::cout << msg4.get()->second.getRawStr() << std::endl;
	}
	// request_t msg4 = parser.parse("\r\n\r\n");

	// if (msg4.get()) {
	// 	std::cout << msg4.get()->first << std::endl;
	// 	std::cout << msg4.get()->second.getRawStr() << std::endl;
	// }
	// request_t msg5 = parser.parse("username=johndo\r\n");

	// if (msg5.get()) {
	// 	std::cout << msg5.get()->first << std::endl;
	// 	std::cout << msg5.get()->second.getRawStr() << std::endl;
	// }

	// request_t msg6 = parser.parse("test");
	// std::string test7 = "G";
	// std::string test8 = "ET";
	// std::string test9 = " ";
	// std::string test10 = "/example HTTP1.1\r\n";

	// parser.parse(test7);
	// parser.parse(test8);
	// parser.parse(test9);
	// parser.parse(test10);

	return 0;
}