#include "utils.hpp"

namespace utils {

	std::string itos(const int& num) {
		std::ostringstream os;
		os << num;
		return os.str();
	}

	std::string getCurTime(const char* format) {
		const std::time_t t = std::time(NULL);
		const std::tm* localTime = std::localtime(&t);

		char buf[42];
		std::strftime(buf, sizeof(buf), format, localTime);

		return DARK_BLUE + std::string(buf) + RESET;
	}

}  // namespace utils