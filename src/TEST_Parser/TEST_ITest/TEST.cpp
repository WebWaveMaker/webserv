#include "TEST.hpp"
#include "ITest.hpp"

int main() {
	ITest* test = new TEST();
	test->run_test();
	return 0;
}
