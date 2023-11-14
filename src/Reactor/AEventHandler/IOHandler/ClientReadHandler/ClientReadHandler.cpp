#include "ClientReadHandler.hpp"

reactor::ClientReadHandler::ClientReadHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}

reactor::ClientReadHandler::~ClientReadHandler() {}

void reactor::ClientReadHandler::handleEvent() {
	std::cout << "hi i'm Client read Handler" << std::endl;
	if (this->getState() == TERMINATE)
		return;
	std::vector<char> buffer(BUFFER_SIZE);
	int readByte = recv(this->getHandle(), buffer.data(), buffer.size() - 1, 0);

	if (readByte == -1) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		return;
	}
	if (readByte == 0) {
		std::cout << "readHandler terminated" << std::endl;
		this->setState(TERMINATE);
		return;
	}
	std::cout << "readByte: " << readByte << std::endl;
	std::cout << buffer.data() << std::endl;

	if (readByte && readByte < BUFFER_SIZE) {
		this->getBuffer().insert(this->getBuffer().end(), buffer.begin(), buffer.begin() + readByte);
		this->getBuffer().push_back('\0');
	}
}
