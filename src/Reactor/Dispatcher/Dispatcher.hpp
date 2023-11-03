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
		void registerIOHandler(sharedData_t sharedData, enum EventType type);

		template <class Factory>
		void registerExeHandler(sharedData_t sharedData, ...);
		void Dispatcher::registerHandler(u::shared_ptr<AEventHandler> handler, enum EventType type);
		void removeHandler(u::shared_ptr<AEventHandler> handler, enum EventType type);
		void addFdToClose(fd_t fd);
		void removeFdToClose(fd_t fd);
		bool isFdMarkedToClose(fd_t fd) const;
		void closePendingFds();
		void handleEvent();
	};
}  // namespace reactor

#endif
