#pragma once
#ifndef TEST_HPP
#define TEST_HPP

#include "ITest.hpp"
#include "ServerConfig.hpp"
#include "shared_ptr.hpp"
#include "utils.hpp"

class TEST : public ITest {
   private:
	ServerConfig _sc;

	void test_getDirectives();
	void test_getErrorPage();
	void test_setDirectives();
	void test_copy_constructor();
	void test_assignment_operator();

   public:
	TEST();
	virtual ~TEST();
	virtual void run_test();
};
