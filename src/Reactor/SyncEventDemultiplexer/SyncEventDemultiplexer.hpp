#pragma once
#ifndef SYNCEVENTDEMULTIPLEXER_HPP
#define SYNCEVENTDEMULTIPLEXER_HPP

#include "SyncEventDemultiplexer.h"

namespace reactor {

	class SyncEventDemultiplexer : public utils::TSingleton<SyncEventDemultiplexer> {
	   private:
		Kqueue* _kq;
		std::map<fd_t, std::time_t> _fdsTime;
		struct timespec _timeout;

		SyncEventDemultiplexer(const SyncEventDemultiplexer& obj);
		SyncEventDemultiplexer& operator=(const SyncEventDemultiplexer& obj);

	   public:
		SyncEventDemultiplexer();
		~SyncEventDemultiplexer();
		void setFdTime(fd_t fd, std::time_t curTime);
		void eraseFdTime(fd_t fd);
		std::time_t getFdTime(fd_t fd);
		void requestEvent(AEventHandler* handler, const enum EventType type);
		void unRequestEvent(AEventHandler* handler, const enum EventType type);
		void unRequestAllEvent(fd_t fd);
		void waitEvents(void);
	};

}  // namespace reactor
#endif
