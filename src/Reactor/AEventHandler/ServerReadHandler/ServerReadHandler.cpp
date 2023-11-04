#include "ServerReadHandler.hpp"

namespace reactor {
	ServerReadHandler::ServerReadHandler(sharedData_t sharedData) : AEventHandler(sharedData) {}
	ServerReadHandler::~ServerReadHandler() {
		std::cout << "ServerReadHandler Destructor called\n";
	}

	void ServerReadHandler::handleEvent() {
		this->setState(ACCEPT);
	}

}  // namespace reactor