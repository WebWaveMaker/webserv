#pragma once
#ifndef TSINGLETON_HPP
#define TSINGLETON_HPP

namespace utils {

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

#endif
