#pragma once
#ifndef CLIENTRESPONSEHANDLER_CPP
#define CLIENTRESPONSEHANDLER_CPP

#include "ClientResponseHandler.h"

namespace reactor {
	class ClientResponseHandler : public AExeHandler {
	   private:
		const request_t _request;
		const utils::shared_ptr<ServerConfig> _serverConfig;
		const utils::shared_ptr<LocationConfig> _locationConfig;
		bool _keepalive;
		Director<sharedData_t> _director;
		bool _registered;

		utils::shared_ptr<IBuilder<sharedData_t> > chooseBuilder();

	   public:
		static utils::shared_ptr<IBuilder<sharedData_t> > createRedirectResponseBuilder(
			const unsigned int statusCode, const std::string& path, reactor::sharedData_t sharedData, request_t request,
			const utils::shared_ptr<ServerConfig>& serverConfig);

		ClientResponseHandler(sharedData_t& sharedData);
		virtual ~ClientResponseHandler();
		virtual void handleEvent();
		std::string findLocationBlock();
	};
}  // namespace reactor
#endif
