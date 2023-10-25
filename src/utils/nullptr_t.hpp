#pragma once
#ifndef NULLPTR_T_HPP
#define NULLPTR_T_HPP

namespace utils {

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
}  // namespace utils

namespace u = utils;
#endif
