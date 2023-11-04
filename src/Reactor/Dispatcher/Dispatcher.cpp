#include "Dispatcher.hpp"

namespace reactor {

	Dispatcher::Dispatcher() : _demultiplexer(SyncEventDemultiplexer::getInstance()) {}

	Dispatcher::~Dispatcher() {}

	template <class Factory>
	void Dispatcher::registerIOHandler(sharedData_t sharedData) {
		const handle_t handle = sharedData.get().fd;
		Factory factory;
		AEventHandler* handler = factory.createHandler(sharedData);

		this->_ioHandlers[handle].push_back(handler);
		this->_handlerIndices[handler] = this->_handlers[handle].size() - 1;
		this->_demultiplexer->requestEvent(handler.get(), sharedData.get().eventType);
	}

	template <class Factory>
	void Dispatcher::registerExeHandler(sharedData_t sharedData, ...) {
		const handle_t handle = sharedData.get().fd;
		Factory factory;
		va_list args;
		va_start(args, sharedData);
		AEventHandler* handler = factory.createHandler(sharedData, args);
		va_end(args);

		this->_exeHandlers[handle].push_back(handler);
		this->_handlerIndices[handler] = this->_exeHandlers[handle].size() - 1;
	}

	void Dispatcher::registerHandler(u::shared_ptr<AEventHandler> handler, enum EventType type) {
		const fd_t handle = handler->getHandle();
	}
	/*IOhandler 하나만 Dispatcher, kevent에서 삭제합니다.*/
	void Dispatcher::removeIOHandler(u::shared_ptr<AEventHandler> handler, enum EventType type) {
		const handle_t handle = handler->getHandle();
		if (this->_ioHandlers.find(handle) != this->_ioHandlers.end()) {
			const size_t index = this->_handlerIndices[handler];
			if (index != this->_ioHandlers[handle].size() - 1) {
				std::swap(this->_ioHandlers[handle][index], this->_ioHandlers[handle].back());
				this->_handlerIndices[this->_ioHandlers[handle][index]] = index;
			}
			this->_demultiplexer->unRequestEvent(this->_ioHandlers[handle].back().get(), type);
			this->_ioHandlers[handle].pop_back();
			this->_handlerIndices.erase(handler);
		}
	}
	/*Exehandler 하나만 Dispatcher에서 삭제합니다.*/
	void Dispatcher::removeExeHandler(u::shared_ptr<AEventHandler> handler) {
		const handle_t handle = handler->getHandle();
		if (this->_exeHandlers.find(handle) != this->_exeHandlers.end()) {
			const size_t index = this->_handlerIndices[handler];
			if (index != this->_exeHandlers[handle].size() - 1) {
				std::swap(this->_exeHandlers[handle][index], this->_exeHandlers[handle].back());
				this->_handlerIndices[this->_exeHandlers[handle][index]] = index;
			}
			this->_exeHandlers[handle].pop_back();
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
	/*연결이 종료 되어야할 clientFd들을 연결 종료합니다.(IOhandler만 모두 삭제합니다.)*/
	void Dispatcher::closePendingFds() {
		for (std::map<fd_t, std::vector<u::shared_ptr<AEventHandler> > >::iterator IOIt = this->_ioHandlers.begin();
			 IOIt != this->_ioHandlers.end(); ++IOIt) {
			std::vector<u::shared_ptr<AEventHandler> >& handlers = IOIt->second;

			for (ssize_t i = 0; i < handlers.size(); ++i) {
				if (handlers[i]->getState() == TERMINATE) {
					this->
				}
			}

			if (!= this->_ioHandlers.end()) {
				this->_demultiplexer->unRequestAllEvent(fd);
				ServerManager::getInstance()->eraseClient(fd);
				std::vector<u::shared_ptr<AEventHandler> > handlersToErase = this->_ioHandlers[fd];

				for (std::vector<u::shared_ptr<AEventHandler> >::iterator handlerIt = handlersToErase.begin();
					 handlerIt != handlersToErase.end(); ++handlerIt)
					this->_handlerIndices.erase(*handlerIt);

				this->_ioHandlers.erase(fd);
				std::cout << fd << " : was closed\n";
			}
		}
		this->_fdsToClose.clear();
	}

	void Dispatcher::handleEvent(void) {
		if (this->_fdsToClose.size() != 0)
			this->closePendingFds();
		_demultiplexer->waitEvents();
		// 벡터 순회하면서 executeHandler-> handleEvent실행.
	}
}  // namespace reactor
