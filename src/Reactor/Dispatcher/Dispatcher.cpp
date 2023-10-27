#include "Dispatcher.hpp"

namespace reactor {

	Dispatcher::Dispatcher() : _demultiplexer(SyncEventDemultiplexer::getInstance()) {}
	Dispatcher::~Dispatcher() {}
	void Dispatcher::registerHander(AEventHandler* handler, enum EventType type) {
		const handle_t handle = handler->getHandle();
		const std::map<handle_t, AEventHandler*>::iterator cit = _handlers.find(handle);

		if (cit == _handlers.end()) {
			_handlers[handle] = handler;
		}
		_demultiplexer->requestEvent(handler, type);
	}

	void Dispatcher::removeHander(AEventHandler* handler, enum EventType type) {
		(void)type;
		const handle_t handle = handler->getHandle();
		_handlers.erase(handle);
		close(handle);
		delete handler;
	}

	void Dispatcher::handleEvent(void) {
		_demultiplexer->waitEvents();
	}
}  // namespace reactor
