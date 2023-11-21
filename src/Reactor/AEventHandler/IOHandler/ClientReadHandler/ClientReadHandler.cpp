#include "ClientReadHandler.hpp"

reactor::ClientReadHandler::ClientReadHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}

reactor::ClientReadHandler::~ClientReadHandler() {}

void reactor::ClientReadHandler::handleEvent() {
	std::cout << "hi i'm Client read Handler" << std::endl;
	if (this->getState() == TERMINATE)
		return;
	std::vector<char> buffer(BUFFER_SIZE);
	int readByte = recv(this->getHandle(), buffer.data(), buffer.size() - 1, 0);

	if (readByte == SYSTEMCALL_ERROR) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		return;
	}
	if (readByte == 0) {
		std::cout << "readHandler terminated" << std::endl;
		this->setState(TERMINATE);
		return;
	}
	this->setReadByte(readByte);
	this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
}
