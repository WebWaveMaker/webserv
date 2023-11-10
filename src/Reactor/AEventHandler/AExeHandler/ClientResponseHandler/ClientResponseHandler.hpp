#pragma once
#ifndef CLIENTRESPONSEHANDLER_CPP
#define CLIENTRESPONSEHANDLER_CPP

#include "ClientResponseHandler.h"

namespace reactor {
	class ClientResponseHandler : public AExeHandler {
	   private:
		const request_t _request;
		Director<sharedData_t> _director;

		utils::shared_ptr<IBuilder<sharedData_t> > chooseBuilder();

	   public:
		ClientResponseHandler(sharedData_t& sharedData, va_list args);
		virtual ~ClientResponseHandler();
		virtual void handleEvent();
	};
}  // namespace reactor
#endif
