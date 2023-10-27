#include "ClientWriteHandler.hpp"

reactor::ClientWriteHandler::ClientWriteHandler(const handle_t fd, utils::shared_ptr<AccessLogger>& accessLogger,
												utils::shared_ptr<ErrorLogger>& errorLogger,
												utils::shared_ptr<std::vector<char> > buf)
	: AEventHandler(fd, accessLogger, errorLogger, buf), _idx(0) {}
reactor::ClientWriteHandler::~ClientWriteHandler() {}

void reactor::ClientWriteHandler::handleEvent() {
	if (this->_buf.get()->empty())
		return;
	const char* buf = this->_buf.get()->data() + this->_idx;
	ssize_t numberOfBytes = send(this->_fd, buf, this->_buf.get()->size() - this->_idx, 0);
	if (numberOfBytes == -1)
		return;
	if (numberOfBytes < static_cast<ssize_t>(this->_buf.get()->size()))
		this->_idx = numberOfBytes;
	if (numberOfBytes == static_cast<ssize_t>(this->_buf.get()->size())) {
		this->_buf.get()->clear();
		reactor::SyncEventDemultiplexer::getInstance()->unRequestEvent(this, EVENT_WRITE);
	}
}