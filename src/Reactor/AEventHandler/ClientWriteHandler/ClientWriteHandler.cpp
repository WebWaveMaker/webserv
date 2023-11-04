#include "ClientWriteHandler.hpp"

reactor::ClientWriteHandler::ClientWriteHandler(sharedData_t sharedData) : AEventHandler(sharedData), _idx(0) {}
reactor::ClientWriteHandler::~ClientWriteHandler() {}

void reactor::ClientWriteHandler::handleEvent() {
	if (this->getBuffer().empty())
		return;
	const std::vector<char>& buffer = this->getBuffer();
	const char* buf = buffer.data() + this->_idx;
	ssize_t numberOfBytes = send(this->getHandle(), buf, this->getBuffer().size() - this->_idx, 0);
	if (numberOfBytes == -1) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "send failed");
		return;
	}
	if (numberOfBytes < static_cast<ssize_t>(this->getBuffer().size()))
		this->_idx = numberOfBytes;
	if (numberOfBytes == static_cast<ssize_t>(this->getBuffer().size())) {
		this->getBuffer().clear();
		reactor::SyncEventDemultiplexer::getInstance()->unRequestEvent(this, this->getType());
	}
}