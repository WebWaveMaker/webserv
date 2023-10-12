#pragma once
#ifndef NULLPTR_HPP
#define NULLPTR_HPP

const class nullptr_t {
   public:
	template <class T>
	operator T*() const {
		return 0;
	}

	template <class C, class T>
	operator T C::*() const {
		return 0;
	}

   private:
	void operator&() const;

} nullptr_t = {};

#endif