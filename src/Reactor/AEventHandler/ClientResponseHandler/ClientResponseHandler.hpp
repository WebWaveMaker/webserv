#pragma once
#ifndef CLIENTRESPONSEHANDLER_CPP
#define CLIENTRESPONSEHANDLER_CPP

#include "ClientResponseHandler.h"

namespace reactor {
	class ClientResponseHandler : public AEventHandler {
	   private:
	   public:
		ClientResponseHandler();
		virtual ~ClientResponseHandler();
	};
}  // namespace reactor
#endif