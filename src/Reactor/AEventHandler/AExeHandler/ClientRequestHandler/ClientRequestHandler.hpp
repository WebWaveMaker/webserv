#pragma once
#ifndef CLIENTREQUESTHANDLER_HPP
#define CLIENTREQUESTHANDLER_HPP

#include "ClientRequestHandler.h"

namespace reactor {

	class ClientRequestHandler : public AExeHandler {
	   private:
		RequestParser _requestParser;
		sharedData_t _writeData;

	   public:
		ClientRequestHandler(sharedData_t& sharedData);
		~ClientRequestHandler();
		RequestParser& getRequest();
		virtual void handleEvent();
	};
}  // namespace reactor
#endif
