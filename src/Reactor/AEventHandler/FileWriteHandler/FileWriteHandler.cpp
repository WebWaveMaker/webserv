#include "FileWriteHandler.hpp"

namespace reactor {
	FileWriteHandler::FileWriteHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}

	FileWriteHandler::~FileWriteHandler() {}

	void FileWriteHandler::handleEvent() {
		if (this->getState() == TERMINATE)
			return;
		std::vector<char>& buffer = this->getBuffer();
		ssize_t numberOfBytes = write(this->getHandle(), buffer.data(), this->getBuffer().size());
		if (numberOfBytes == -1) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "write failed");
			this->setState(TERMINATE);
			return;
		}
		if (numberOfBytes == 0) {  // 이 조건문이 맞는지 모르겠음.
			this->setState(TERMINATE);
			return;
		}
		if (numberOfBytes <= static_cast<ssize_t>(buffer.size()))
			buffer.erase(buffer.begin(), buffer.begin() + numberOfBytes);
		// if (numberOfBytes == static_cast<ssize_t>(this->getBuffer().size())) {
		// 	this->getBuffer().clear();
		// 	// SyncEventDemultiplexer::getInstance()->unRequestEvent(this, this->getType()); ??
		// }
	}
}  // namespace reactor