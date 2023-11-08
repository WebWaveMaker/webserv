#pragma once
#ifndef SYNCEVENTDEMULTIPLEXER_HPP
#define SYNCEVENTDEMULTIPLEXER_HPP

#include "SyncEventDemultiplexer.h"

namespace reactor {

	class SyncEventDemultiplexer : public utils::TSingleton<SyncEventDemultiplexer> {
	   private:
		Kqueue* _kq;

		SyncEventDemultiplexer(const SyncEventDemultiplexer& obj);
		SyncEventDemultiplexer& operator=(const SyncEventDemultiplexer& obj);

	   public:
		SyncEventDemultiplexer();
		~SyncEventDemultiplexer();
		void requestEvent(AEventHandler* handler, const enum EventType type);
		void unRequestEvent(AEventHandler* handler, const enum EventType type);
		void unRequestAllEvent(fd_t fd);
		void waitEvents(void);
	};

}  // namespace reactor
#endif
