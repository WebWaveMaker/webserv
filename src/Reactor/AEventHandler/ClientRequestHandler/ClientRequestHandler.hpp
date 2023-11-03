#pragma once
#ifndef CLIENTREQUESTHANDLER_HPP
#define CLIENTREQUESTHANDLER_HPP

#include "ClientRequestHandler.h"

namespace reactor {

	class ClientRequestHandler : public AEventHandler {
	   private:
		request_t _request;

	   public:
		ClientRequestHandler(sharedData_t sharedData, va_list args);
		~ClientRequestHandler();
		request_t getRequest() const;
		virtual void handleEvent();
	};
}  // namespace reactor
#endif
