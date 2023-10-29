#include "SyncEventDemultiplexer.hpp"

namespace reactor {
	SyncEventDemultiplexer::SyncEventDemultiplexer() : _kq(Kqueue::getInstance()) {}
	SyncEventDemultiplexer::~SyncEventDemultiplexer() {}

	void SyncEventDemultiplexer::requestEvent(AEventHandler* handler, const enum EventType type) {
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
	void SyncEventDemultiplexer::unRequestEvent(AEventHandler* handler, const enum EventType type) {
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
		std::cout << "ChangeList Size: " << _kq->getChangeList().size() << std::endl;
		for (int i = 0; i < eventNum; ++i)
			static_cast<AEventHandler*>(_kq->getkEventList()[i].udata)->handleEvent();
	};
}  // namespace reactor
