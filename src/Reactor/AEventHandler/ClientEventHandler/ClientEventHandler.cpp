#include "ClientEventHandler.hpp"

ClientEventHandler::ClientEventHandler(handle_t handleFd, ICallback* callback, Client* client)
	: _handleFd(handleFd), _callback(callback), _client(client) {
	std::cout << "ClientEventHandler constructor called\n";
}

handle_t ClientEventHandler::getHandle() const {
	return (this->_handleFd);
}

Client* ClientEventHandler::getClient() const {
	return (this->_client);
}

void ClientEventHandler::handleRead() {
	std::vector<char> buffer(BUFFER_SIZE);

	while (this->_client->getReqParser()->getState() != RESOLVE) {
		int readByte = read(this->_handleFd, buffer.data(), buffer.size() - 1);

		if (readByte > 0) {
			request_t request = this->_client->getReqParser()->parse(std::string(buffer.data()));
			if (request.get()->first == DONE)
				this->_client->executeRequest();
		} else if (this->_client->getReqParser()->getState() != RESOLVE)
			break;
	}
	int readByte = read(this->_handleFd, buffer.data(), buffer.size());
	(void)readByte;
}

void ClientEventHandler::handleWrite() {}

void ClientEventHandler::handleError() {
	reactor::Dispatcher::getInstance()->removeHander(this, EVENT_READ);
	reactor::Dispatcher::getInstance()->removeHander(this, EVENT_WRITE);
	this->_callback->eraseClient(this->_handleFd);
}

ClientEventHandler::~ClientEventHandler() {
	// this->_callback->execute(this->_handleFd);
	delete this->_client;  // Server에서 ClientEventHandler의 소멸자를 호출하면 삭제한다.
}
