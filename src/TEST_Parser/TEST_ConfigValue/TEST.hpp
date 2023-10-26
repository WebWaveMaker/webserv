#pragma once
#ifndef TEST_HPP
#define TEST_HPP

#include "ConfigValue.hpp"
#include "ITest.hpp"

class TEST : public ITest {
   private:
	ConfigValue _cvBool;
	ConfigValue _cvUint;
	ConfigValue _cvString;
	ConfigValue _cvLog;
	ConfigValue _cvMedVec;
	ConfigValue _cvStrVec;

	void test_asBool();
	void test_asUint();
	void test_asString();
	void test_asLog();
	void test_asStrVec();
	void test_asMedVec();
	void test_copy_constructor();
	void test_assignment_operator();

   public:
	TEST();
	virtual ~TEST();
	virtual void run_test();
}

#endif