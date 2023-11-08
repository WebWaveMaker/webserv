#include "Test.hpp"

Test::Test()
	: _cvBool(true),
	  _cvUint((unsigned int)1),
	  _cvString("test"),
	  _cvLog(std::pair<std::string, LogLevels>("test", LOG_ERROR)),
	  _cvMedVec(std::vector<HttpMethods>(1, GET)),
	  _cvStrVec(std::vector<std::string>(1, "test")) {}

Test::~Test() {}

void Test::run_test() {
	std::cout << "Test ConfigValue class run_test()" << std::endl;
	this->test_asBool();
	this->test_asUint();
	this->test_asString();
	this->test_asLog();
	this->test_asStrVec();
	this->test_asMedVec();
	this->test_copy_constructor();
	this->test_assignment_operator();
	system("leaks a.out");
	std::cout << "all tests passed" << std::endl;
}

void Test::test_asBool() {
	std::cout << "test_asBool: ";
	assert(_cvBool.asBool() == true);
	_cvBool = false;
	assert(_cvBool.asBool() == false);
	std::cout << "OK" << std::endl;
}

void Test::test_asUint() {
	std::cout << "test_asUint: ";
	assert(_cvUint.asUint() == 1);
	_cvUint = 2;
	assert(_cvUint.asUint() == 2);
	std::cout << "OK" << std::endl;
}

void Test::test_asString() {
	std::cout << "test_asString: ";
	assert(_cvString.asString() == "test");
	_cvString = "test2";
	assert(_cvString.asString() == "test2");
	std::cout << "OK" << std::endl;
}

void Test::test_asLog() {
	std::cout << "test_asLog: ";
	assert(_cvLog.asLog().first == "test");
	assert(_cvLog.asLog().second == LOG_ERROR);
	_cvLog = std::pair<std::string, LogLevels>("test2", LOG_DEBUG);
	assert(_cvLog.asLog().first == "test2");
	assert(_cvLog.asLog().second == LOG_DEBUG);
	std::cout << "OK" << std::endl;
}

void Test::test_asStrVec() {
	std::cout << "test_asStrVec: ";
	assert(_cvStrVec.asStrVec()[0] == "test");
	_cvStrVec = std::vector<std::string>(1, "test2");
	assert(_cvStrVec.asStrVec()[0] == "test2");
	std::cout << "OK" << std::endl;
}

void Test::test_asMedVec() {
	std::cout << "test_asMedVec: ";
	assert(_cvMedVec.asMedVec()[0] == GET);
	_cvMedVec = std::vector<HttpMethods>(1, POST);
	assert(_cvMedVec.asMedVec()[0] == POST);
	std::cout << "OK" << std::endl;
}

void Test::test_copy_constructor() {
	std::cout << "test_copy_constructor: ";
	ConfigValue cvBoolCopy(_cvBool);
	assert(cvBoolCopy.asBool() == _cvBool.asBool());
	ConfigValue cvUintCopy(_cvUint);
	assert(cvUintCopy.asUint() == _cvUint.asUint());
	ConfigValue cvStringCopy(_cvString);
	assert(cvStringCopy.asString() == _cvString.asString());
	ConfigValue cvLogCopy(_cvLog);
	assert(cvLogCopy.asLog().first == _cvLog.asLog().first);
	assert(cvLogCopy.asLog().second == _cvLog.asLog().second);
	ConfigValue cvStrVecCopy(_cvStrVec);
	assert(cvStrVecCopy.asStrVec()[0] == _cvStrVec.asStrVec()[0]);
	ConfigValue cvMedVecCopy(_cvMedVec);
	assert(cvMedVecCopy.asMedVec()[0] == _cvMedVec.asMedVec()[0]);
	std::cout << "OK" << std::endl;
}

void Test::test_assignment_operator() {
	std::cout << "test_assignment_operator: ";
	ConfigValue cvBoolCopy = _cvBool;
	assert(cvBoolCopy.asBool() == _cvBool.asBool());
	ConfigValue cvUintCopy = _cvUint;
	assert(cvUintCopy.asUint() == _cvUint.asUint());
	ConfigValue cvStringCopy = _cvString;
	assert(cvStringCopy.asString() == _cvString.asString());
	ConfigValue cvLogCopy = _cvLog;
	assert(cvLogCopy.asLog().first == _cvLog.asLog().first);
	assert(cvLogCopy.asLog().second == _cvLog.asLog().second);
	ConfigValue cvStrVecCopy = _cvStrVec;
	assert(cvStrVecCopy.asStrVec()[0] == _cvStrVec.asStrVec()[0]);
	ConfigValue cvMedVecCopy = _cvMedVec;
	assert(cvMedVecCopy.asMedVec()[0] == _cvMedVec.asMedVec()[0]);
	std::cout << "OK" << std::endl;
}
