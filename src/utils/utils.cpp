#include "utils.hpp"

std::string utils::itos(const int& num) {
	std::ostringstream os;
	os << num;
	return os.str();
}