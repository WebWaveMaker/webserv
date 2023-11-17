#include "PipeReadHandler.hpp"

namespace reactor {
	PipeReadHandler::PipeReadHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}
	PipeReadHandler::~PipeReadHandler() {}
	void PipeReadHandler::handleEvent() {
		if (this->getState() == TERMINATE || this->getState() == RESOLVE)
			return;
		std::vector<char> buffer(BUFFER_SIZE);
		ssize_t readByte = read(this->getHandle(), buffer.data(), buffer.size() - 1);
		if (readByte == SYSTEMCALL_ERROR) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "read fail");
			this->setState(TERMINATE);
			return;
		}
		if (readByte == 0) {
			this->setState(RESOLVE);
			return;
		}
		std::cout << "readByte: " << readByte << std::endl;
		std::cout << "buffer: " << buffer.data() << std::endl;
		this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
	}

}  // namespace reactor