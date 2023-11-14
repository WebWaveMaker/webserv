#include "ServerReadHandler.hpp"

namespace reactor {
	ServerReadHandler::ServerReadHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}
	ServerReadHandler::~ServerReadHandler() {}

	void ServerReadHandler::handleEvent() {
		this->setState(ACCEPT);
	}

}  // namespace reactor