#include "SyncEventDemultiplexer.hpp"

namespace reactor {
	SyncEventDemultiplexer::SyncEventDemultiplexer() : _kq(Kqueue::getInstance()) {}
	SyncEventDemultiplexer::~SyncEventDemultiplexer() {}

	void SyncEventDemultiplexer::requestEvent(IEventHandler* handler, const enum EventType type) {
		const handle_t handle = handler->getHandle();

		switch (type) {
			case EVENT_READ:
				_kq->AddEventOnChangeList(handle, EVFILT_READ, EV_ADD, 0, 0, handler);
				break;

			case EVENT_WRITE:
				_kq->AddEventOnChangeList(handle, EVFILT_WRITE, EV_ADD, 0, 0, handler);
				break;

			default:
				break;
		}
	}
	void SyncEventDemultiplexer::unRequestEvent(IEventHandler* handler, const enum EventType type) {
		const handle_t handle = handler->getHandle();

		switch (type) {
			case EVENT_READ:
				_kq->registerEvent(handle, EVFILT_READ, EV_DELETE, 0, 0, u::nullptr_t);
				break;

			case EVENT_WRITE:
				_kq->registerEvent(handle, EVFILT_WRITE, EV_DELETE, 0, 0, u::nullptr_t);
				break;

			default:
				break;
		}
	}

	void SyncEventDemultiplexer::waitEvents(void) {
		const int eventNum = kevent(_kq->getFd(), &_kq->getChangeList()[0], _kq->getChangeList().size(),
									&_kq->getkEventList()[0], _kq->getkEventList().size(), u::nullptr_t);
		if (eventNum == -1)
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		_kq->getChangeList().clear();
		for (int i = 0; i < eventNum; ++i) {
			const struct kevent event = _kq->getkEventList()[i];
			IEventHandler* handler = static_cast<IEventHandler*>(event.udata);
			if (event.flags & EV_EOF) {};
			if (event.flags & EV_ERROR)
				handler->handleError();
			if (event.filter == EVFILT_READ)
				handler->handleRead();
			if (event.filter == EVFILT_WRITE)
				handler->handleWrite();
		}
	};
}  // namespace reactor
