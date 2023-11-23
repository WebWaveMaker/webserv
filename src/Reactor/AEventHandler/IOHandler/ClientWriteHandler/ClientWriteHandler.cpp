#include "ClientWriteHandler.hpp"

namespace reactor {
	ClientWriteHandler::ClientWriteHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}
	ClientWriteHandler::~ClientWriteHandler() {}

	void ClientWriteHandler::handleEvent() {
		// std::cout << "hi i'm write Handler" << std::endl;
		if (this->getState() == TERMINATE || this->getState() == RESOLVE || this->getBuffer().size() == 0)
			return;
		std::vector<char>& buffer = this->getBuffer();
		std::cerr << buffer.data();
		ssize_t numberOfBytes = send(this->getHandle(), buffer.data(), this->getBuffer().size(), 0);
		if (numberOfBytes == SYSTEMCALL_ERROR)
			return;
		buffer.erase(buffer.begin(), buffer.begin() + numberOfBytes);
	}
}  // namespace reactor
