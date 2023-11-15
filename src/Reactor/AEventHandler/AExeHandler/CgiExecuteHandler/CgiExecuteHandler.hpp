#pragma once
#ifndef CGIEXECUTEHANDLER_HPP
#define CGIEXECUTEHANDLER_HPP

#include "CgiExecuteHandler.h"

namespace reactor {
	class CgiExecuteHandler : public AExeHandler {
	   private:
		int _sv[2];
		request_t _request;
		sharedData_t _readSharedData;
		const utils::shared_ptr<ServerConfig> _serverConfig;
		const utils::shared_ptr<LocationConfig> _locationConfig;

		CgiExecuteHandler(const CgiExecuteHandler& obj);
		CgiExecuteHandler& operator=(const CgiExecuteHandler& obj);
		bool validCgiPath();

	   public:
		CgiExecuteHandler(sharedData_t& sharedData, va_list args);
		~CgiExecuteHandler();
		virtual void handleEvent();
	};
}  // namespace reactor

// Start Line: GET / HTTP/1.1
// Headers:
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
// Accept-Encoding: gzip, deflate
// Accept-Language: en-us
// Connection: keep-alive
// Host: localhost:4240
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.6.1 Safari/605.1.15
// Body:
// Error Code: 0

#endif
