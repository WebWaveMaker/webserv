#pragma once
#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "Dispatcher.h"

namespace reactor {

	class Dispatcher : public utils::TSingleton<Dispatcher> {
	   private:
		SyncEventDemultiplexer* _demultiplexer;
		std::map<fd_t, std::vector<u::shared_ptr<AEventHandler> > > _ioHandlers;
		std::map<fd_t, std::vector<u::shared_ptr<AEventHandler> > > _exeHandlers;
		std::map<u::shared_ptr<AEventHandler>, size_t> _handlerIndices;
		std::set<fd_t> _fdsToClose;

		Dispatcher(const Dispatcher& obj);
		Dispatcher& operator=(const Dispatcher& obj);

	   public:
		Dispatcher();
		~Dispatcher();
		template <class Factory>
		void registerIOHandler(sharedData_t& sharedData) {
			const handle_t handle = sharedData.get()->fd;
			Factory factory;
			u::shared_ptr<AEventHandler> handler = factory.createHandler(sharedData);

			this->_ioHandlers[handle].push_back(handler);
			this->_handlerIndices[handler] = this->_ioHandlers[handle].size() - 1;
			this->_demultiplexer->requestEvent(handler.get(), sharedData.get()->type);
		}

		template <class Factory>
		void registerExeHandler(sharedData_t sharedData, ...) {
			const handle_t handle = sharedData->fd;
			Factory factory;
			va_list args;
			va_start(args, sharedData);
			u::shared_ptr<AEventHandler> handler = factory.createHandler(sharedData, args);
			va_end(args);

			this->_exeHandlers[handle].push_back(handler);
			this->_handlerIndices[handler] = this->_exeHandlers[handle].size() - 1;
		}
		void removeIOHandler(fd_t fd, enum EventType type);
		void removeExeHandler(u::shared_ptr<AEventHandler> handler);
		void addFdToClose(fd_t fd);
		void removeFdToClose(fd_t fd);
		bool isFdMarkedToClose(fd_t fd) const;
		void closePendingFds();
		void exeHandlerexe();
		void handleEvent();
	};
}  // namespace reactor

#endif
