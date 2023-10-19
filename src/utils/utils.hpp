#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include "utils.h"

namespace utils {
	std::string itos(const int& num);
	std::string getCurTime(const char* format);

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

	template <class T>
	class TSingleton {

	   public:
		static T* getInstance() {
			if (_instance == nullptr_t) {
				_instance = new T;
				std::atexit(deleteInstance);
			}
			return _instance;
		}

	   protected:
		TSingleton() {}
		virtual ~TSingleton() {}

		static void deleteInstance() {
			if (_instance != nullptr_t)
				delete _instance;
			_instance = nullptr_t;
		}

	   private:
		static T* _instance;
	};

	template <class T>
	T* TSingleton<T>::_instance = nullptr_t;

}  // namespace utils

namespace u = utils;

#endif
