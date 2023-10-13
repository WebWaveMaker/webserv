#pragma once
#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include "Kqueue.h"

class Kqueue {
   private:
	fd_t _kq;

	Kqueue(const Kqueue& obj);
	Kqueue& operator=(const Kqueue& obj);
   public:
	Kqueue(void);
	~Kqueue();
	void registerEvent(const uintptr_t ident, const int16_t filter, const uint16_t flags, const uint32_t fflags,
					   const intptr_t data, void* udata);
};

#endif
