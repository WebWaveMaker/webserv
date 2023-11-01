#include "Dispatcher.hpp"

namespace reactor {

	Dispatcher::Dispatcher() : _demultiplexer(SyncEventDemultiplexer::getInstance()) {}

	Dispatcher::~Dispatcher() {}

	void Dispatcher::registerHandler(u::shared_ptr<AEventHandler> handler, enum EventType type) {
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

	void Dispatcher::addFdToClose(fd_t fd) {
		this->_fdsToClose.insert(fd);
	}

	void Dispatcher::removeFdToClose(fd_t fd) {
		this->_fdsToClose.erase(fd);
	}

	bool Dispatcher::isFdMarkedToClose(fd_t fd) const {
		return (this->_fdsToClose.find(fd) != this->_fdsToClose.end());
	}

	void Dispatcher::closePendingFds() {
		for (std::set<fd_t>::iterator fdIt = this->_fdsToClose.begin(); fdIt != this->_fdsToClose.end(); ++fdIt) {
			fd_t fd = *fdIt;

			if (this->_handlers.find(fd) != this->_handlers.end()) {
				this->_demultiplexer->unRequestAllEvent(fd);
				ServerManager::getInstance()->eraseClient(fd);
				std::vector<u::shared_ptr<AEventHandler> > handlersToErase = this->_handlers[fd];
				// 핸들러 목록 순회하면서 핸들러의 인덱스를 _handlerIndices에서 제거합니다.
				for (std::vector<u::shared_ptr<AEventHandler> >::iterator handlerIt = handlersToErase.begin();
					 handlerIt != handlersToErase.end(); ++handlerIt)
					this->_handlerIndices.erase(*handlerIt);

				// _handlers에서 해당 fd의 핸들러 목록을 완전히 제거합니다.
				this->_handlers.erase(fd);
				std::cout << fd << " : was closed\n";
			}
		}
		this->_fdsToClose.clear();
	}

	void Dispatcher::handleEvent(void) {
		if (this->_fdsToClose.size() != 0)
			this->closePendingFds();
		_demultiplexer->waitEvents();
	}
}  // namespace reactor
