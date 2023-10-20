#include "utils.hpp"

namespace utils {

	std::string itos(const int& num) {
		std::ostringstream os;
		os << num;
		return os.str();
	}
}  // namespace utils
