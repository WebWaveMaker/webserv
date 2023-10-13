#include "Kqueue.hpp"

Kqueue::Kqueue() : _kq(kqueue()) {
	if (this->_kq == -1) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		throw(errno);
	};
}

Kqueue::~Kqueue() {
	close(this->_kq);
}

void Kqueue::registerEvent(const uintptr_t ident, const int16_t filter, const uint16_t flags, const uint32_t fflags,
						   const intptr_t data, void* udata) {
	struct kevent event;

	EV_SET(&event, ident, filter, flags, fflags, data, udata);
	if (kevent(this->_kq, &event, 1, nullptr_t, 0, nullptr_t) == -1) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		throw(errno);
	}
};
