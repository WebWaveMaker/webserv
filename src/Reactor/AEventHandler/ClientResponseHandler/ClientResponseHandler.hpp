#pragma once
#ifndef CLIENTRESPONSEHANDLER_CPP
#define CLIENTRESPONSEHANDLER_CPP

#include "ClientResponseHandler.h"

namespace reactor {
	class ClientResponseHandler : public AEventHandler {
	   private:
		request_t _request;

	   public:
		ClientResponseHandler(sharedData_t sharedData, va_list args);
		virtual ~ClientResponseHandler();
		virtual void handleEvent();
	};
}  // namespace reactor
#endif