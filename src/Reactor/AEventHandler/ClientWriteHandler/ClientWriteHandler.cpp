#include "ClientWriteHandler.hpp"

namespace reactor {
	ClientWriteHandler::ClientWriteHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}
	ClientWriteHandler::~ClientWriteHandler() {}

	void ClientWriteHandler::handleEvent() {
		if (this->getState() == TERMINATE || this->getState() == RESOLVE || this->getBuffer().size() == 0)
			return;
		std::vector<char>& buffer = this->getBuffer();
		ssize_t numberOfBytes = send(this->getHandle(), buffer.data(), this->getBuffer().size(), 0);
		if (numberOfBytes == -1) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "send failed");
			return;
		}
		buffer.erase(buffer.begin(), buffer.begin() + numberOfBytes);
	}
}  // namespace reactor