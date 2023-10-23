#include "ConfigValue.hpp"
#include "Parser.h"

int main() {

	std::cout << "Testing constructors" << std::endl;
	ConfigValue cv;
	std::cerr << "cv.getType() = " << cv.getType() << std::endl;
	std::cerr << "cv.asNULL() = " << cv.asNULL() << std::endl;
	assert(cv.asNULL() == NULL);

	ConfigValue cvBool(true);
	std::cerr << "cvBool.getType() = " << cvBool.getType() << std::endl;
	std::cerr << "cvBool.asBool() = " << cvBool.asBool() << std::endl;
	assert(cvBool.asBool() == true);

	ConfigValue cvUint((unsigned int)42);
	std::cerr << "cvUint.getType() = " << cvUint.getType() << std::endl;
	std::cerr << "cvUint.asUint() = " << cvUint.asUint() << std::endl;
	assert(cvUint.asUint() == 42);

	std::string s = "Hello World!";
	ConfigValue cvString(s);
	std::cerr << "cvString.getType() = " << cvString.getType() << std::endl;
	std::cerr << "cvString.asString() = " << cvString.asString() << std::endl;
	assert(cvString.asString() == s);

	std::pair<std::string, LogLevels> l("Hello World!", LOG_ERROR);
	ConfigValue cvLog(l);
	std::cerr << "cvLog.getType() = " << cvLog.getType() << std::endl;
	std::cerr << "cvLog.asLog() = " << cvLog.asLog().first << ", " << cvLog.asLog().second << std::endl;
	assert(cvLog.asLog().first == "Hello World!");
	assert(cvLog.asLog().second == LOG_ERROR);

	std::vector<HttpMethod> v({GET, POST, DELETE, PUT});
	ConfigValue cvMedVec(v);
	std::cerr << "cvMedVec.getType() = " << cvMedVec.getType() << std::endl;
	std::cerr << "cvMedVec.asMedVec() = ";
	for (HttpMethod m : cvMedVec.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	assert(cvMedVec.asMedVec() == v);

	std::vector<std::string> v2({"Hello", "World", "!"});
	ConfigValue cvStrVec(v2);
	std::cerr << "cvStrVec.getType() = " << cvStrVec.getType() << std::endl;
	std::cerr << "cvStrVec.asStrVec() = ";
	for (std::string s : cvStrVec.asStrVec()) {
		std::cerr << s << ", ";
	}
	std::cerr << std::endl;
	assert(cvStrVec.asStrVec() == v2);
	std::cout << std::endl;
	std::cout << "Testing copy constructor" << std::endl;
	ConfigValue cv2(cv);
	std::cerr << "cv2.getType() = " << cv2.getType() << std::endl;
	std::cerr << "cv2.asNULL() = " << cv2.asNULL() << std::endl;
	assert(cv2.asNULL() == NULL);

	ConfigValue cvBool2(cvBool);
	std::cerr << "cvBool2.getType() = " << cvBool2.getType() << std::endl;
	std::cerr << "cvBool2.asBool() = " << cvBool2.asBool() << std::endl;
	assert(cvBool2.asBool() == true);

	ConfigValue cvUint2(cvUint);
	std::cerr << "cvUint2.getType() = " << cvUint2.getType() << std::endl;
	std::cerr << "cvUint2.asUint() = " << cvUint2.asUint() << std::endl;
	assert(cvUint2.asUint() == 42);

	ConfigValue cvString2(cvString);
	std::cerr << "cvString2.getType() = " << cvString2.getType() << std::endl;
	std::cerr << "cvString2.asString() = " << cvString2.asString() << std::endl;
	assert(cvString2.asString() == s);

	ConfigValue cvLog2(cvLog);
	std::cerr << "cvLog2.getType() = " << cvLog2.getType() << std::endl;
	std::cerr << "cvLog2.asLog() = " << cvLog2.asLog().first << ", " << cvLog2.asLog().second << std::endl;
	assert(cvLog2.asLog().first == "Hello World!");
	assert(cvLog2.asLog().second == LOG_ERROR);

	ConfigValue cvMedVec2(cvMedVec);
	std::cerr << "cvMedVec2.getType() = " << cvMedVec2.getType() << std::endl;
	std::cerr << "cvMedVec2.asMedVec() = ";
	for (HttpMethod m : cvMedVec2.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	assert(cvMedVec2.asMedVec() == v);

	ConfigValue cvStrVec2(cvStrVec);
	std::cerr << "cvStrVec2.getType() = " << cvStrVec2.getType() << std::endl;
	std::cerr << "cvStrVec2.asStrVec() = ";
	for (std::string s : cvStrVec2.asStrVec()) {
		std::cerr << s << ", ";
	}
	std::cerr << std::endl;
	assert(cvStrVec2.asStrVec() == v2);
	std::cout << std::endl;
	std::cout << "Testing assignment operator" << std::endl;
	cv2 = cv;
	std::cerr << "cv2.getType() = " << cv2.getType() << std::endl;
	std::cerr << "cv2.asNULL() = " << cv2.asNULL() << std::endl;
	assert(cv2.asNULL() == NULL);

	cvBool2 = cvBool;
	std::cerr << "cvBool2.getType() = " << cvBool2.getType() << std::endl;
	std::cerr << "cvBool2.asBool() = " << cvBool2.asBool() << std::endl;
	assert(cvBool2.asBool() == true);

	cvUint2 = cvUint;
	std::cerr << "cvUint2.getType() = " << cvUint2.getType() << std::endl;
	std::cerr << "cvUint2.asUint() = " << cvUint2.asUint() << std::endl;
	assert(cvUint2.asUint() == 42);

	cvString2 = cvString;
	std::cerr << "cvString2.getType() = " << cvString2.getType() << std::endl;
	std::cerr << "cvString2.asString() = " << cvString2.asString() << std::endl;
	assert(cvString2.asString() == s);

	cvLog2 = cvLog;
	std::cerr << "cvLog2.getType() = " << cvLog2.getType() << std::endl;
	std::cerr << "cvLog2.asLog() = " << cvLog2.asLog().first << ", " << cvLog2.asLog().second << std::endl;
	assert(cvLog2.asLog().first == "Hello World!");
	assert(cvLog2.asLog().second == LOG_ERROR);

	cvMedVec2 = cvMedVec;
	std::cerr << "cvMedVec2.getType() = " << cvMedVec2.getType() << std::endl;
	std::cerr << "cvMedVec2.asMedVec() = ";
	for (HttpMethod m : cvMedVec2.asMedVec()) {
		std::cerr << m << ", ";
	}
	std::cerr << std::endl;
	assert(cvMedVec2.asMedVec() == v);

	cvStrVec2 = cvStrVec;
	std::cerr << "cvStrVec2.getType() = " << cvStrVec2.getType() << std::endl;
	std::cerr << "cvStrVec2.asStrVec() = ";
	for (std::string s : cvStrVec2.asStrVec()) {
		std::cerr << s << ", ";
	}
	std::cerr << std::endl;
	assert(cvStrVec2.asStrVec() == v2);

	std::cout << std::endl;
	std::cout << "Testing destructor" << std::endl;
	cv.~ConfigValue();
	cvBool.~ConfigValue();
	cvUint.~ConfigValue();
	cvString.~ConfigValue();
	cvLog.~ConfigValue();
	cvMedVec.~ConfigValue();
	cvStrVec.~ConfigValue();
	cv2.~ConfigValue();
	cvBool2.~ConfigValue();
	cvUint2.~ConfigValue();
	cvString2.~ConfigValue();
	cvLog2.~ConfigValue();
	cvMedVec2.~ConfigValue();
	cvStrVec2.~ConfigValue();
	std::cout << std::endl;

	system("leaks a.out | grep leaked");
}
