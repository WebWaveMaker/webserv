#include "ClientReadHandler.hpp"

reactor::ClientReadHandler::ClientReadHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}

reactor::ClientReadHandler::~ClientReadHandler() {}

void reactor::ClientReadHandler::handleEvent() {
	// std::cerr << "read handler\n";
	if (this->getState() == TERMINATE)
		return;
	std::vector<char> buffer(BUFFER_SIZE);
	int readByte = read(this->getHandle(), buffer.data(), BUFFER_SIZE - 1);

	if (readByte == SYSTEMCALL_ERROR) {
		this->setState(TERMINATE);
		std::cerr << "errFd: " << this->getHandle() << std::endl;
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		return;
	}
	if (readByte == 0) {
		this->setState(TERMINATE);
		return;
	}
	// std::cerr << std::string(buffer.begin(), buffer.begin() + readByte) << std::endl;
	this->setReadByte(readByte);
	this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
}
