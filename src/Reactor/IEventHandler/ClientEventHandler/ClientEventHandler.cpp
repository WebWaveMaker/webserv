#include "ClientEventHandler.hpp"

ClientEventHandler::ClientEventHandler(handle_t handleFd, Client* client) : _handleFd(handleFd), _client(client) {
	std::cout << "ClientEventHandler constructor called\n";
}

handle_t ClientEventHandler::getHandle() const {
	return (this->_handleFd);
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
}