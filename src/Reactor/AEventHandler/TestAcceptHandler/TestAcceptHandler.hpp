#pragma once
#ifndef TESTACCEPTHANDLER_HPP
#define TESTACCEPTHANDLER_HPP

#include "TestAcceptHandler.h"

class TestAcceptHandler {
   private:
	TestAcceptHandler();
	TestAcceptHandler(const TestAcceptHandler& obj);
	TestAcceptHandler& operator=(const TestAcceptHandler& obj);

   public:
	static void printClientInfo(const Client* client);
	static void printAllClientInfo(const std::map<int, u::shared_ptr<Client> > clients);
	~TestAcceptHandler();
};

#endif
