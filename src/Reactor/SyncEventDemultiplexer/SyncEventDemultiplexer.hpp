#pragma once
#ifndef SYNCEVENTDEMULTIPLEXER_HPP
#define SYNCEVENTDEMULTIPLEXER_HPP

namespace reactor {

	class SyncEventDemultiplexer {
	   private:
	   public:
		SyncEventDemultiplexer();
		SyncEventDemultiplexer(const SyncEventDemultiplexer& obj);
		~SyncEventDemultiplexer();
		SyncEventDemultiplexer& operator=(const SyncEventDemultiplexer& obj);
	};

}  // namespace reactor
#endif
