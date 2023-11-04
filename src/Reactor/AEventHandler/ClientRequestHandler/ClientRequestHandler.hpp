#pragma once
#ifndef CLIENTREQUESTHANDLER_HPP
#define CLIENTREQUESTHANDLER_HPP

#include "ClientRequestHandler.h"

namespace reactor {

	class ClientRequestHandler : public AEventHandler {
	   private:
		RequestParser _request;

	   public:
		ClientRequestHandler(sharedData_t sharedData, va_list args);
		~ClientRequestHandler();
		RequestParser& getRequest();
		virtual void handleEvent();
	};
}  // namespace reactor
#endif
