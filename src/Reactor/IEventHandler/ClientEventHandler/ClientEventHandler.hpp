#pragma once
#ifndef CLIENTEVENTHANDLER_HPP
#define CLIENTEVENTHANDLER_HPP

class ClientEventHandler {
   private:
   public:
	ClientEventHandler();
	ClientEventHandler(const ClientEventHandler& obj);
	~ClientEventHandler();
	ClientEventHandler& operator=(const ClientEventHandler& obj);
};

#endif

ClientEventHandler::ClientEventHandler() {}
ClientEventHandler::ClientEventHandler(const ClientEventHandler& obj) {}
ClientEventHandler::~ClientEventHandler() {}
ClientEventHandler& ClientEventHandler::operator=(const ClientEventHandler& obj) {}