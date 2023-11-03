#include "ClientReadHandler.hpp"

reactor::ClientReadHandler::ClientReadHandler(sharedData_t sharedData) : AEventHandler(sharedData) {}

reactor::ClientReadHandler::~ClientReadHandler() {}

void reactor::ClientReadHandler::handleEvent() {
	if (this->getState() == TERMINATE) {
		std::cout << "return\n";
		return;
	}
	std::vector<char> buffer(BUFFER_SIZE);
	int readByte = recv(this->_fd, buffer.data(), buffer.size() - 1, 0);

	if (readByte == -1) {
		this->_errorLogger.get()->log("recv fail", __func__, LOG_ERROR, u::nullptr_t);
		return;
	}
	if (readByte == 0) {
		std::cout << this->getHandle() << "addFdToClose\n";
		this->setState(TERMINATE);
		return;
	}
	std::cout << "readByte: " << readByte << std::endl;
	std::cout << buffer.data() << std::endl;

	if (readByte > 0 && readByte < BUFFER_SIZE)
		this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
}
