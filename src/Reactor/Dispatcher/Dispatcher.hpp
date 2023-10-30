#pragma once
#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "Dispatcher.h"

namespace reactor {

	class Dispatcher : public utils::TSingleton<Dispatcher> {
	   private:
		SyncEventDemultiplexer* _demultiplexer;
		std::map<fd_t, u::shared_ptr<AEventHandler> > _handlers;
		// std::map<u::shared_ptr<AEventHandler>, int> _handlerIndices;

		Dispatcher(const Dispatcher& obj);
		Dispatcher& operator=(const Dispatcher& obj);

	   public:
		Dispatcher();
		~Dispatcher();
		void registerHander(u::shared_ptr<AEventHandler> handler, enum EventType type);
		void removeHander(u::shared_ptr<AEventHandler> handler, enum EventType type);
		void handleEvent();
	};
}  // namespace reactor

#endif
