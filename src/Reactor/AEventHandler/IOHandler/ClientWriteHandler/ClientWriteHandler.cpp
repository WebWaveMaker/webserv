#include "ClientWriteHandler.hpp"

namespace reactor {
	ClientWriteHandler::ClientWriteHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}
	ClientWriteHandler::~ClientWriteHandler() {}

	void ClientWriteHandler::handleEvent() {
		if (this->getState() == TERMINATE || this->getState() == RESOLVE || this->getBuffer().empty()) {
			return;
		}
		std::vector<char>& buffer = this->getBuffer();
		ssize_t numberOfBytes = write(this->getHandle(), buffer.data(), this->getBuffer().size());
		if (numberOfBytes == SYSTEMCALL_ERROR)
			return;
		buffer.erase(buffer.begin(), buffer.begin() + numberOfBytes);
	}
}  // namespace reactor
