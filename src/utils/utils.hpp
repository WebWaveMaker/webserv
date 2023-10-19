#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include "utils.h"

namespace utils {
	std::string itos(const int& num);

	const class nullptr_t {
	   public:
		template <typename T>
		operator T*() const {
			return 0;
		}
		template <typename C, typename T>
		operator T C::*() const {
			return 0;
		}

	   private:
		void operator&() const;

	} nullptr_t = {};
 } 

#endif
