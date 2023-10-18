#pragma once
#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

namespace reactor {

	class Dispatcher {
	   private:
	   public:
		Dispatcher();
		Dispatcher(const Dispatcher& obj);
		~Dispatcher();
		Dispatcher& operator=(const Dispatcher& obj);
	};
}  // namespace reactor

#endif