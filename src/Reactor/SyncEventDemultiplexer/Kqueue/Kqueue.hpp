#pragma once
#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include "Kqueue.h"

class Kqueue {
   private:
	fd_t _kq;
	fd_set _client_fds;
	fd_set _server_fds;

	Kqueue(const Kqueue& obj);
	Kqueue& operator=(const Kqueue& obj);

   public:
	Kqueue(void);
	~Kqueue();
	bool registerEvent(const uintptr_t ident, const int16_t filter, const uint16_t flags, const uint32_t fflags,
					   const intptr_t data, void* udata);
	bool deregisterEvent(const uintptr_t ident, const int16_t filter);	// udata?

	fd_set getClientFds(void) const;
	fd_set getServerFds(void) const;
	void clearFds(const enum FdSetType type, const fd_t fd);
	void setFds(const enum FdSetType type, const fd_t fd);
	enum FdSetType confirmFdSetType(const uintptr_t ident);
};

#endif
