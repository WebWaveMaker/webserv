#include "Kqueue.hpp"

Kqueue::Kqueue() : _fd(kqueue()), _kEventList(8), _changeList() {
	if (this->_fd == -1)
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
}

Kqueue::~Kqueue() {
	close(this->_fd);
}

void Kqueue::registerEvent(const uintptr_t ident, const int16_t filter, const uint16_t flags, const uint32_t fflags,
						   const intptr_t data, void* udata) {
	struct kevent event;

	EV_SET(&event, ident, filter, flags, fflags, data, udata);
	if (kevent(this->_fd, &event, 1, u::nullptr_t, 0, u::nullptr_t) == -1)
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
};

void Kqueue::AddEventOnChangeList(const uintptr_t ident, const int16_t filter, const uint16_t flags,
								  const uint32_t fflags, const intptr_t data, void* udata) {
	struct kevent event;

	EV_SET(&event, ident, filter, flags, fflags, data, udata);
	this->_changeList.push_back(event);
}

fd_t Kqueue::getFd(void) const {
	return this->_fd;
}

std::vector<struct kevent>& Kqueue::getChangeList(void) {
	return this->_changeList;
}

std::vector<struct kevent>& Kqueue::getkEventList(void) {
	return this->_kEventList;
}
