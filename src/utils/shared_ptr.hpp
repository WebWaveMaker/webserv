#pragma once
#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include "nullptr_t.hpp"
#include "utils.h"

namespace utils {

	template <class C>
	class shared_ptr {
	   private:
		C* _ptr;
		mutable std::size_t* _cnt;

		void swap(shared_ptr& lhs) throw() {
			std::swap(_ptr, lhs._ptr);
			std::swap(_cnt, lhs._cnt);
		};

	   public:
		typedef C element_type;

		shared_ptr() : _ptr(u::nullptr_t), _cnt(u::nullptr_t){};

		explicit shared_ptr(C* ptr) : _ptr(ptr), _cnt(new std::size_t(1)) {
			if (_cnt == NULL) {
				delete ptr;
				throw std::bad_alloc();
			}
		};

		~shared_ptr() throw() { release(); };

		shared_ptr(const shared_ptr& ptr) : _ptr(ptr._ptr), _cnt(ptr._cnt) {
			if (_cnt && _ptr) {
				++(*_cnt);
			}
		};

		shared_ptr& operator=(shared_ptr ptr) throw() {
			swap(ptr);
			return *this;
		};

		void release() throw() {
			if (_cnt != NULL) {
				--(*_cnt);
				if (*_cnt == 0) {
					delete _ptr;
					delete _cnt;
					_ptr = u::nullptr_t;
					_cnt = u::nullptr_t;
				}
			}
		};

		template <class U>
		void acquire(U* p) const {
			if (p != NULL) {
				if (_cnt == NULL) {
					try {
						_cnt = new std::size_t(1);
					} catch (std::bad_alloc&) {
						delete p;
						throw;	// rethrow the std::bad_alloc
					}
				} else {
					++(*_cnt);
				}
			}
		}

		template <class U>
		shared_ptr(const shared_ptr<U>& ptr, C* p) : _ptr(p) {
			ptr.acquire(p);
			_cnt = ptr.getCnt();
		}

		std::size_t* getCnt(void) const { return _cnt; }
		C* get(void) const throw() { return _ptr; }
		C& operator*() const throw() { return *_ptr; }
		C* operator->() const throw() { return _ptr; }
	};

	// static cast of shared_ptr
	template <class T, class U>
	u::shared_ptr<T> static_pointer_cast(const u::shared_ptr<U>& ptr) throw() {
		return u::shared_ptr<T>(ptr, static_cast<typename u::shared_ptr<T>::element_type*>(ptr.get()));
	}

	// dynamic cast of shared_ptr
	template <class T, class U>
	u::shared_ptr<T> dynamic_pointer_cast(const u::shared_ptr<U>& ptr) throw() {
		T* p = dynamic_cast<typename u::shared_ptr<T>::element_type*>(ptr.get());
		if (NULL != p) {
			return u::shared_ptr<T>(ptr, p);
		} else {
			return u::shared_ptr<T>();
		}
	}

	// comparaison operators
	template <class T, class U>
	bool operator==(const shared_ptr<T>& l, const shared_ptr<U>& r) throw()	 // never throws
	{
		return (l.get() == r.get());
	}
	template <class T, class U>
	bool operator!=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw()	 // never throws
	{
		return (l.get() != r.get());
	}
	template <class T, class U>
	bool operator<=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw()	 // never throws
	{
		return (l.get() <= r.get());
	}
	template <class T, class U>
	bool operator<(const shared_ptr<T>& l, const shared_ptr<U>& r) throw()	// never throws
	{
		return (l.get() < r.get());
	}
	template <class T, class U>
	bool operator>=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw()	 // never throws
	{
		return (l.get() >= r.get());
	}
	template <class T, class U>
	bool operator>(const shared_ptr<T>& l, const shared_ptr<U>& r) throw()	// never throws
	{
		return (l.get() > r.get());
	}

}  // namespace utils

namespace u = utils;

#endif
