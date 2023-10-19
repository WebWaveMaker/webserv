#pragma once
#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include "Kqueue.h"

class Kqueue : public utils::TSingleton<Kqueue> {
   private:
	fd_t _fd;
	std::vector<struct kevent> _kEventList;
	std::vector<struct kevent> _changeList;

	Kqueue(const Kqueue& obj);
	Kqueue& operator=(const Kqueue& obj);

   public:
	//OCF
	Kqueue(void);
	~Kqueue();

	// method
	void registerEvent(const uintptr_t ident, const int16_t filter, const uint16_t flags, const uint32_t fflags,
					   const intptr_t data, void* udata);
	void AddEventOnChangeList(const uintptr_t ident, const int16_t filter, const uint16_t flags, const uint32_t fflags,
							  const intptr_t data, void* udata);

	// getter
	fd_t getFd(void) const;
	std::vector<struct kevent>& getChangeList(void);
	std::vector<struct kevent>& getkEventList(void);
};

#endif
