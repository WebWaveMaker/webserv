#include "FileReadHandler.hpp"

namespace reactor {
	FileReadHandler::FileReadHandler(sharedData_t& sharedData)
		: AEventHandler(sharedData), _fp(fdopen(this->getHandle(), "r")), _fd(this->getHandle()) {
		const fd_t fd = fileno(this->_fp);
		if (fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)	 // error
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "fcntl failed");
		this->setHandler(fd);
	}

	FileReadHandler::~FileReadHandler() {
		fclose(this->_fp);
		close(this->_fd);
	}

	void FileReadHandler::handleEvent() {
		std::cout << "hi i'm FilereadHandler" << std::endl;
		if (this->getState() == TERMINATE || this->getState() == RESOLVE)
			return;
		std::vector<char> buffer(BUFFER_SIZE);
		std::size_t readByte = fread(buffer.data(), sizeof(char), BUFFER_SIZE - 1, this->_fp);

		if (ferror(this->_fp)) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "read fail");
			this->setState(TERMINATE);
			return;
		}
		if (feof(this->_fp)) {
			this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
			this->setState(RESOLVE);
			return;
		}
		this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
	}
}  // namespace reactor
