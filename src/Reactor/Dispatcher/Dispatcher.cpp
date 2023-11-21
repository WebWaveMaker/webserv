#include "Dispatcher.hpp"

namespace reactor {

	Dispatcher::Dispatcher() : _demultiplexer(SyncEventDemultiplexer::getInstance()) {}

	Dispatcher::~Dispatcher() {}
	/*IOhandler 하나만 Dispatcher, kevent에서 삭제합니다.*/
	void Dispatcher::removeIOHandler(fd_t fd, enum EventType type) {
		if (this->_ioHandlers.find(fd) == this->_ioHandlers.end())
			return;
		std::vector<u::shared_ptr<AEventHandler> > handlers = this->_ioHandlers.find(fd)->second;
		u::shared_ptr<AEventHandler> handler;

		for (std::vector<u::shared_ptr<AEventHandler> >::iterator it = handlers.begin(); it != handlers.end(); ++it) {
			if (it->get()->getType() == type)
				handler = *it;
		}
		if (!(handler.get()))
			return;

		size_t index = this->_handlerIndices[handler];

		if (index != this->_ioHandlers[fd].size() - 1) {
			std::swap(this->_ioHandlers[fd][index], this->_ioHandlers[fd].back());
			this->_handlerIndices[this->_ioHandlers[fd][index]] = index;
		}

		this->_demultiplexer->unRequestEvent(this->_ioHandlers[fd].back().get(), type);
		this->_ioHandlers[fd].pop_back();
		this->_handlerIndices.erase(handler);
	}
	/*Exehandler 하나만 Dispatcher에서 삭제합니다.*/
	void Dispatcher::removeExeHandler(AEventHandler* handler) {
		const handle_t handle = handler->getHandle();
		for (size_t i = 0; i < this->_exeHandlers[handle].size(); ++i) {
			if (this->_exeHandlers[handle][i].get() == handler) {
				this->_removeHandlers.push_back(this->_exeHandlers[handle][i]);
				return;
			}
		}
		// this->_pendingChanges.push_back(HandlerChange(HandlerChange::Remove, handler));
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
	bool Dispatcher::isWriting(fd_t fd) const {

		std::map<fd_t, std::vector<u::shared_ptr<AEventHandler> > >::const_iterator it = this->_exeHandlers.find(fd);
		if (it != this->_exeHandlers.end()) {
			const std::vector<u::shared_ptr<AEventHandler> >& handlers = it->second;
			for (std::vector<u::shared_ptr<AEventHandler> >::const_iterator handlerIt = handlers.begin();
				 handlerIt != handlers.end(); ++handlerIt) {
				if ((*handlerIt)->getType() == EVENT_WRITE) {
					return true;
				}
			}
		}
		return false;
	}
	/*연결이 종료 되어야할 clientFd들을 연결 종료합니다.(IOhandler만 모두 삭제합니다.)*/
	void Dispatcher::closePendingFds() {
		for (std::set<fd_t>::iterator it = this->_fdsToClose.begin(); it != this->_fdsToClose.end(); ++it) {
			if (this->_ioHandlers.find(*it) != this->_ioHandlers.end()) {
				this->_demultiplexer->unRequestAllEvent(*it);
				this->_demultiplexer->eraseFdTime(*it);
				ServerManager::getInstance()->eraseClient(*it);

				std::vector<u::shared_ptr<AEventHandler> > handlersToErase = this->_ioHandlers[*it];
				for (std::vector<u::shared_ptr<AEventHandler> >::iterator handlerIt = handlersToErase.begin();
					 handlerIt != handlersToErase.end(); ++handlerIt) {
					(*handlerIt)->setState(TERMINATE);
					this->_handlerIndices.erase(*handlerIt);
				}

				this->_ioHandlers.erase(*it);
				std::cout << *it << " : was closed\n";
			}
		}
		this->_fdsToClose.clear();
	}

	void Dispatcher::applyHandlersChanges() {
		for (size_t i = 0; i < this->_addHandlers.size(); ++i) {
			const handle_t handle = this->_addHandlers[i]->getHandle();
			this->_exeHandlers[handle].push_back(this->_addHandlers[i]);
			this->_handlerIndices[this->_addHandlers[i]] = this->_exeHandlers[handle].size() - 1;
		}
		for (size_t i = 0; i < this->_removeHandlers.size(); ++i) {
			const handle_t handle = this->_removeHandlers[i]->getHandle();
			if (this->_exeHandlers.find(handle) != this->_exeHandlers.end()) {
				const size_t index = this->_handlerIndices[this->_removeHandlers[i]];
				if (index < this->_exeHandlers[handle].size()) {
					if (index != this->_exeHandlers[handle].size() - 1) {
						std::swap(this->_exeHandlers[handle][index], this->_exeHandlers[handle].back());
						this->_handlerIndices[this->_exeHandlers[handle][index]] = index;
					}
					this->_exeHandlers[handle].pop_back();
					this->_handlerIndices.erase(this->_removeHandlers[i]);
				}
			}
		}
		this->_addHandlers.clear();
		this->_removeHandlers.clear();
	}

	void Dispatcher::exeHandlerexe() {
		for (std::map<fd_t, std::vector<u::shared_ptr<AEventHandler> > >::iterator it = this->_exeHandlers.begin();
			 it != this->_exeHandlers.end(); ++it) {
			for (std::vector<u::shared_ptr<AEventHandler> >::iterator evnetIt = it->second.begin();
				 evnetIt != it->second.end(); ++evnetIt) {
				evnetIt->get()->handleEvent();
			}
		}
		this->applyHandlersChanges();
		for (size_t i = 0; i < this->_fdsToClose.size(); ++i) {
			for (size_t handlerIdx = 0; i < this->_exeHandlers[i].size(); ++handlerIdx)
				this->_removeHandlers.push_back(this->_exeHandlers[i][handlerIdx]);
		}
		this->applyHandlersChanges();
	}

	void Dispatcher::handleEvent(void) {
		this->_demultiplexer->waitEvents();
		// if (!this->_exeHandlers.empty())
		this->exeHandlerexe();
		// if (!this->_fdsToClose.empty())
		this->closePendingFds();
	}
}  // namespace reactor
