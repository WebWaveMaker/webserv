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

		CgiExecuteHandler(const CgiExecuteHandler& obj);
		CgiExecuteHandler& operator=(const CgiExecuteHandler& obj);

	   public:
		CgiExecuteHandler(sharedData_t& sharedData, va_list args);
		~CgiExecuteHandler();
		virtual void handleEvent();
	};
}  // namespace reactor

#endif
