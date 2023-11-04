#include "FileReadHandler.hpp"

namespace reactor {

	FileReadHandler::FileReadHandler(sharedData_t sharedData) : AEventHandler(sharedData) {}
	FileReadHandler::~FileReadHandler() {}

	void FileReadHandler::handleEvent() {
		if (this->getState() == TERMINATE)
			return;
		std::vector<char> buffer(BUFFER_SIZE);
		int readByte = read(this->getHandle(), buffer.data(), buffer.size() - 1);

		if (readByte == -1) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "read fail");
			this->setState(TERMINATE);
			return;
		}
		if (readByte == 0) {
			this->setState(TERMINATE);
			return;
		}

		std::cout << "readByte: " << readByte << std::endl;
		std::cout << "buffer: " << buffer.data() << std::endl;
		if (readByte && readByte < BUFFER_SIZE)
			this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
	}
}  // namespace reactor