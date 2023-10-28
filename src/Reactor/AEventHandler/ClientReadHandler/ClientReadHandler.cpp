#include "ClientReadHandler.hpp"

reactor::ClientReadHandler::ClientReadHandler(const handle_t fd, const utils::shared_ptr<AccessLogger>& accessLogger,
											  const utils::shared_ptr<ErrorLogger>& errorLogger,
											  const utils::shared_ptr<Client>& client)
	: AEventHandler(fd, accessLogger, errorLogger), _client(client) {}

reactor::ClientReadHandler::~ClientReadHandler() {}

#include "ClientWriteHandler.hpp"

void reactor::ClientReadHandler::handleEvent() {
	std::vector<char> buffer(BUFFER_SIZE);
	int readByte = recv(this->_fd, buffer.data(), buffer.size() - 1, 0);

	if (readByte == -1) {
		this->_errorLogger.get()->log("recv fail", __func__, LOG_ERROR, u::nullptr_t);
		return;
	}
	if (this->_client->getReqParser()->getState() == RESOLVE) {
		reactor::Dispatcher::getInstance()->removeHander(this, EVENT_READ);
		std::cout << "byebye" << std::endl;
	}

	std::cout << "readByte: " << readByte << std::endl;
	std::cout << buffer.data() << std::endl;

	if (readByte > 0 && readByte < BUFFER_SIZE) {
		request_t request = this->_client->getReqParser().get()->parse(std::string(buffer.data()));
		if (request.get()->first == DONE)
			this->_client->executeRequest();
	}
}