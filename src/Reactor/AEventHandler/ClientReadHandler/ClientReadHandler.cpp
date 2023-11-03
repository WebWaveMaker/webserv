#include "ClientReadHandler.hpp"

reactor::ClientReadHandler::ClientReadHandler(sharedData_t sharedData, utils::shared_ptr<RequestParser> req)
	: AEventHandler(sharedData), _req(req) {}

reactor::ClientReadHandler::~ClientReadHandler() {}

void reactor::ClientReadHandler::handleEvent() {
	if (reactor::Dispatcher::getInstance()->isFdMarkedToClose(this->getHandle())) { // ? Dispatcher에 접근하면 안되는데...
		std::cout << "return\n";
		return;
	}
	std::vector<char> buffer(BUFFER_SIZE);
	int readByte = recv(this->getHandle(), buffer.data(), buffer.size() - 1, 0);

	if (readByte == -1) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__, "recv fail");
		return;
	}
	if (readByte == 0) {
		std::cout << this->getHandle() << "addFdToClose\n";
		reactor::Dispatcher::getInstance()->addFdToClose(this->getHandle());
	}
	std::cout << "readByte: " << readByte << std::endl;
	std::cout << buffer.data() << std::endl;

	if (readByte > 0 && readByte < BUFFER_SIZE) {
		request_t request = this->_req.get()->parse(std::string(buffer.data()));
	}
}
