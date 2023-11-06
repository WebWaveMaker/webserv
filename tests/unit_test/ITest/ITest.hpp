#pragma once
#ifndef TEST_ACONFIG_HPP
#define TEST_ACONFIG_HPP

class ITest {
   public:
	virtual ~ITest() {}
	virtual void run_test() = 0;
};

#endif