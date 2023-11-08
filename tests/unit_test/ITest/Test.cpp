#include "Test.hpp"
#include "ITest.hpp"

int main() {
	ITest* test = new Test();
	test->run_test();
	return 0;
}
