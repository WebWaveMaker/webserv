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
	int readByte = read(this->_handleFd, buffer.data(), buffer.size());

	if (readByte < 0) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
	} else {
		std::string content(buffer.begin(), buffer.end());
		// RequestParser(content);
		// 실행처리
		// Response 처리
	}
	// partial read 발생시 read 루프 및 req 완료 확인 로직 필요
}

void ClientEventHandler::handleWrite() {}

void ClientEventHandler::handleError() {
	// Client Error 발생시 callback함수를 호출해서 disconnect한다.
	// this->_callback->execute(this->_handleFd);
}

ClientEventHandler::~ClientEventHandler() {
	// this->_callback->execute(this->_handleFd);
	delete this->_client;  // Server에서 ClientEventHandler의 소멸자를 호출하면 삭제한다.
}