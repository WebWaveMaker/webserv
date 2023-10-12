#include "Kqueue.hpp"

Kqueue::Kqueue() : _kq(kqueue()) {
	if (this->_kq == -1) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		throw(errno);
	};
	FD_ZERO(&this->_clinet_fds);
	FD_ZERO(&this->_server_fds);
}

Kqueue::~Kqueue() {}

bool Kqueue::registerEvent(const uintptr_t ident, const int16_t filter, const uint16_t flags, const uint32_t fflags,
						   const intptr_t data, void* udata) {
	struct kevent event;

	EV_SET(&event, ident, filter, flags, fflags, data, udata);
	if (kevent(this->_kq, &event, 1, nullptr_t, 0, nullptr_t) == -1) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		throw(errno);
	}
};
bool Kqueue::deregisterEvent(const uintptr_t ident, const int16_t filter){};

fd_set Kqueue::getClientFds(void) const {
	return this->_client_fds;
};

fd_set Kqueue::getServerFds(void) const {
	return this->_server_fds;
};

void Kqueue::clearFds(const enum FdSetType type, const fd_t fd) {
	switch (type) {
		case CLIENT_FD:
			FD_CLR(fd, &this->_client_fds);
			break;

		case SERVER_FD:
			FD_CLR(fd, &this->_server_fds);
			break;

		default:
			break;
	}
};
void Kqueue::setFds(const enum FdSetType type, const fd_t fd) {
	switch (type) {
		case CLIENT_FD:
			FD_SET(fd, &this->_client_fds);
			break;

		case SERVER_FD:
			FD_SET(fd, &this->_server_fds);
			break;

		default:
			break;
	}
};

enum FdSetType Kqueue::confirmFdSetType(const uintptr_t ident) {
	if (FD_ISSET(ident, &this->_client_fds))
		return CLIENT_FD;
	else if (FD_ISSET(ident, &this->_server_fds))
		return SERVER_FD;
	else
};