#include "FileWriteHandler.hpp"

namespace reactor {
	FileWriteHandler::FileWriteHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}

	FileWriteHandler::~FileWriteHandler() {}

	void FileWriteHandler::handleEvent() {
		if (this->getState() == TERMINATE || this->getState() == RESOLVE ||
			this->getBuffer().size() == 0)	// executeHandler가 이 핸들러의 상태를 변경한다.
			return;
		std::vector<char>& buffer = this->getBuffer();
		ssize_t numberOfBytes = write(this->getHandle(), buffer.data(), this->getBuffer().size());
		if (numberOfBytes == -1) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "write failed");
			this->setState(TERMINATE);
			return;
		}
		buffer.erase(buffer.begin(), buffer.begin() + numberOfBytes);
	}
}  // namespace reactor