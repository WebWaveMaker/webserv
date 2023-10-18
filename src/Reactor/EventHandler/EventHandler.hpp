#pragma once
#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

namespace reactor {

	class EventHandler {
	   private:
	   public:
		EventHandler();
		EventHandler(const EventHandler& obj);
		~EventHandler();
		EventHandler& operator=(const EventHandler& obj);
	};

}  // namespace reactor
#endif
