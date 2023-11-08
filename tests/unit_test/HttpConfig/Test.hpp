#pragma once
#ifndef TEST_HTTPCONFIG_HPP
#define TEST_HTTPCONFIG_HPP

#include "HttpConfig.hpp"
#include "ITest.hpp"

class Test : public ITest {
   private:
	HttpConfig* _httpConfig;
	void test_getDirectives();
	void test_getErrorPage();
	void test_setDirectives();

   public:
	Test();
	virtual ~Test();
	virtual void run_test();
};
#endif	// TEST_HTTPCONFIG_HPP