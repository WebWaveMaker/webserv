#include "FileReadHandler.hpp"

namespace reactor {

	FileReadHandler::FileReadHandler(sharedData_t& sharedData)
		: AEventHandler(sharedData), _fp(fdopen(this->getHandle(), "r")) {}
	FileReadHandler::~FileReadHandler() {}

	void FileReadHandler::handleEvent() {
		if (this->getState() == TERMINATE)
			return;
		std::vector<char> buffer(BUFFER_SIZE);
		std::size_t readByte = fread(buffer.data(), sizeof(char), BUFFER_SIZE - 1, this->_fp);

		if (ferror(this->_fp)) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "read fail");
			this->setState(TERMINATE);
			return;
		}
		if (feof(this->_fp)) {
			this->setState(TERMINATE);
			return;
		}

		std::cout << "readByte: " << readByte << std::endl;
		std::cout << "buffer: " << buffer.data() << std::endl;
		if (readByte && readByte < BUFFER_SIZE)
			this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
	}
}  // namespace reactor