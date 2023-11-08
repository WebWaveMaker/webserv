#include "SyncEventDemultiplexer.hpp"

namespace reactor {
	SyncEventDemultiplexer::SyncEventDemultiplexer() : _kq(Kqueue::getInstance()) {}
	SyncEventDemultiplexer::~SyncEventDemultiplexer() {}

	void SyncEventDemultiplexer::requestEvent(AEventHandler* handler, const enum EventType type) {
		this->_kq->AddEventOnChangeList(handler->getHandle(), type, EV_ADD, 0, 0, handler);
	}
	void SyncEventDemultiplexer::unRequestEvent(AEventHandler* handler, const enum EventType type) {
		this->_kq->registerEvent(handler->getHandle(), type, EV_DELETE, 0, 0, u::nullptr_t);
	}
	void SyncEventDemultiplexer::unRequestAllEvent(fd_t fd) {
		this->_kq->AddEventOnChangeList(fd, EVFILT_READ, EV_DELETE, 0, 0, u::nullptr_t);
		this->_kq->AddEventOnChangeList(fd, EVFILT_WRITE, EV_DELETE, 0, 0, u::nullptr_t);
	}

	void SyncEventDemultiplexer::waitEvents(void) {
		const int eventNum =
			kevent(this->_kq->getFd(), &this->_kq->getChangeList()[0], this->_kq->getChangeList().size(),
				   &this->_kq->getkEventList()[0], this->_kq->getkEventList().size(), u::nullptr_t);
		if (eventNum == -1)
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		this->_kq->getChangeList().clear();
		for (int i = 0; i < eventNum; ++i) {
			std::cout << "\nevent fd: " << this->_kq->getkEventList()[i].ident << std::endl;
			if (this->_kq->getkEventList()[i].flags & EV_ERROR)
				continue;
			static_cast<AEventHandler*>(this->_kq->getkEventList()[i].udata)->handleEvent();
		}
	};
}  // namespace reactor
