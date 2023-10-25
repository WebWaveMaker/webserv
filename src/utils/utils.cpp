#include "utils.hpp"

std::string utils::itos(const int& num) {
	std::ostringstream os;
	os << num;
	return os.str();
}

extern unsigned int utils::stringToDecimal(const std::string& value) {
	if (value.empty()) {
		throw std::runtime_error("error: empty string");
	}

	char unit = value[value.size() - 1];
	std::string numberPart = value;

	// 단위 검사
	if (isdigit(unit) == false) {
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
std::string getCurTime(const char* format) {
	const std::time_t t = std::time(NULL);
	const std::tm* localTime = std::localtime(&t);

	char buf[42];
	std::strftime(buf, sizeof(buf), format, localTime);

	return DARK_BLUE + std::string(buf) + RESET;
}

}  // namespace utils
