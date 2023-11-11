#include "FileReadHandler.hpp"

namespace reactor {
	FileReadHandler::FileReadHandler(sharedData_t& sharedData)
		: AEventHandler(sharedData), _fp(fdopen(this->getHandle(), "r")) {
		const fd_t fd = fileno(this->_fp);
		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)	 // error
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "fcntl failed");
		this->setHandler(fd);
	}

	FileReadHandler::~FileReadHandler() {
		if (this->_fp != utils::nullptr_t)
			fclose(this->_fp);
	}

	void FileReadHandler::handleEvent() {
		if (this->getState() == TERMINATE || this->getState() == RESOLVE)
			return;
		std::vector<char> buffer(BUFFER_SIZE);
		std::size_t readByte = fread(buffer.data(), sizeof(char), BUFFER_SIZE - 1, this->_fp);

		if (ferror(this->_fp)) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "read fail");
			this->setState(TERMINATE);
			return;
		}
		if (feof(this->_fp) && readByte == 0) {
			this->setState(RESOLVE);
			return;
		}
		std::cout << "readByte: " << readByte << std::endl;
		std::cout << "buffer: " << buffer.data() << std::endl;
		this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
	}
}  // namespace reactor