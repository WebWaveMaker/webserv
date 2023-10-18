#include "Parser.h"

extern unsigned int stringToDecimal(const std::string& value) {
	if (value.empty()) {
		throw std::runtime_error("error: empty string");
	}

	char unit = value[value.size() - 1];
	std::string numberPart = value;

	// 단위 검사
	if (!isdigit(unit)) {
		numberPart = value.substr(0, value.size() - 1);
		switch (unit) {
			case 'm':
			case 'M':
				return atoi(numberPart.c_str()) * 1048576;
			case 'k':
			case 'K':
				return atoi(numberPart.c_str()) * 1024;
			default:
				throw std::runtime_error("error: invalid unit");
		}
	}

	return atoi(value.c_str());
}