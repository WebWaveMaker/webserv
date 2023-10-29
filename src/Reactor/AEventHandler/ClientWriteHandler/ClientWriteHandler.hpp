#pragma once
#ifndef CLIENTWRITEHANDLER_HPP
#define CLIENTWRITEHANDLER_HPP

#include "ClientWriteHandler.h"

namespace reactor {

	class ClientWriteHandler : public AEventHandler {
	   private:
		ClientWriteHandler& operator=(const ClientWriteHandler& obj);
		ClientWriteHandler(const ClientWriteHandler& obj);
		ssize_t _idx;

	   public:
		ClientWriteHandler(const handle_t fd, const utils::shared_ptr<AccessLogger>& accessLogger,
						   const utils::shared_ptr<ErrorLogger>& errorLogger,
						   utils::shared_ptr<std::vector<char> > buf);
		~ClientWriteHandler();
		virtual void handleEvent();
	};

}  // namespace reactor

#endif