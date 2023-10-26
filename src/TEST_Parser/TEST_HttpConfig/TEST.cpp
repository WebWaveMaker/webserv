#include "TEST.hpp"

TEST::TEST() {
	_httpConfig = new HttpConfig();
}

TEST::~TEST() {
	delete _httpConfig;
}

void TEST::run_test() {
	std::cout << "TEST HttpConfig class run_test()" << std::endl;
	this->test_getDirectives();
	this->test_getErrorPage();
	this->test_setDirectives();
	this->test_copy_constructor();
	this->test_assignment_operator();
	this->test_destructor();
	system("leaks a.out");
	std::cout << "all tests passed" << std::endl;
}

void TEST::test_getDirectives() {
	std::cout << "test_getDirectives: ";
	assert(_httpConfig->getDirectives(SENDFILE).asBool() == DEF_SENDFILE);
	assert(_httpConfig->getDirectives(KEEPALIVE_TIMEOUT).asUint() == DEF_KEEPALIVE_TIMEOUT);
	assert(_httpConfig->getDirectives(DEFAULT_TYPE).asString() == DEF_DEFAULT_TYPE);
	assert(_httpConfig->getDirectives(ERROR_LOG).asLog().first == DEF_ERROR_LOG_PATH);
	assert(_httpConfig->getDirectives(ERROR_LOG).asLog().second == LOG_ERROR);
	assert(_httpConfig->getDirectives(CLIENT_MAX_BODY_SIZE).asUint() == DEF_CLIENT_MAX_BODY_SIZE);
	assert(_httpConfig->getDirectives(LISTEN).asUint() == DEF_LISTEN);
	assert(_httpConfig->getDirectives(SERVER_NAME).asString() == DEF_SERVER_NAME);
	assert(_httpConfig->getDirectives(ROOT).asString() == DEF_ROOT);
	assert(_httpConfig->getDirectives(AUTOINDEX).asBool() == DEF_AUTOINDEX);
	assert(_httpConfig->getDirectives(INDEX).asStrVec()[0] == DEF_INDEX);
	assert(_httpConfig->getDirectives(LIMIT_EXCEPT).asMedVec().size() == 4);
	std::cout << "OK" << std::endl;
}

void TEST::test_getErrorPage() {
	std::cout << "test_getErrorPage: ";
	assert(_httpConfig->getErrorPage(DEF_ERROR_CODE) == DEF_ERROR_PAGE);
	assert(_httpConfig->getErrorPage(DEF_ERROR_CODE + 1) == DEF_ERROR_PAGE);
	std::cout << "OK" << std::endl;
}

void TEST::test_setDirectives() {
	std::cout << "test_setDirectives: ";
	_httpConfig->setDirectives("sendfile", std::vector<std::string>(1, "off"));
	assert(_httpConfig->getDirectives(SENDFILE).asBool() == false);
	_httpConfig->setDirectives("sendfile", std::vector<std::string>(1, "on"));
	assert(_httpConfig->getDirectives(SENDFILE).asBool() == true);
	_httpConfig->setDirectives("keepalive_timeout", std::vector<std::string>(1, "100"));
	assert(_httpConfig->getDirectives(KEEPALIVE_TIMEOUT).asUint() == 100);
	_httpConfig->setDirectives("default_type", std::vector<std::string>(1, "text/html"));
	assert(_httpConfig->getDirectives(DEFAULT_TYPE).asString() == "text/html");
	_httpConfig->setDirectives("error_log", std::vector<std::string>(2, "logs/error.log", "debug"));
	assert(_httpConfig->getDirectives(ERROR_LOG).asLog().first == "logs/error.log");
	assert(_httpConfig->getDirectives(ERROR_LOG).asLog().second == LOG_DEBUG);
	_httpConfig->setDirectives("client_max_body_size", std::vector<std::string>(1, "2m"));
	assert(_httpConfig->getDirectives(CLIENT_MAX_BODY_SIZE).asUint() == 1000000);
	_httpConfig->setDirectives("error_page", std::vector<std::string>(3, "500", "400", "testerrorpage"));
	std::cout << "OK" << std::endl;
}