#include "Dispatcher.hpp"

namespace reactor {

	Dispatcher::Dispatcher() : _demultiplexer(SyncEventDemultiplexer::getInstance()) {}

	Dispatcher::~Dispatcher() {}

	void Dispatcher::registerHander(u::shared_ptr<AEventHandler> handler, enum EventType type) {
		const fd_t handle = handler->getHandle();

		if (this->_handlers.find(handle) == this->_handlers.end()) {
			this->_handlers[handle].push_back(handler);
			this->_handlerIndices[handler] = this->_handlers[handle].size() - 1;
		}
		this->_demultiplexer->requestEvent(handler.get(), type);
	}

	void Dispatcher::removeHander(u::shared_ptr<AEventHandler> handler, enum EventType type) {
		const handle_t handle = handler->getHandle();
		if (this->_handlers.find(handle) != this->_handlers.end()) {
			const size_t index = this->_handlerIndices[handler];
			if (index != this->_handlers[handle].size() - 1) {
				std::swap(this->_handlers[handle][index], this->_handlers[handle].back());
				this->_handlerIndices[this->_handlers[handle][index]] = index;
			}
			this->_demultiplexer->unRequestEvent(this->_handlers[handle].back().get(), type);
			this->_handlers[handle].pop_back();
			this->_handlerIndices.erase(handler);
		}
	}

	void Dispatcher::handleEvent(void) {
		_demultiplexer->waitEvents();
	}
}  // namespace reactor
