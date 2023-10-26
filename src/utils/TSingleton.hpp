#pragma once
#ifndef TSINGLETON_HPP
#define TSINGLETON_HPP

namespace utils {

	template <class T>
	class TSingleton {

	   public:
		static T* getInstance() {
			if (_instance == NULL) {
				_instance = new T;
				std::atexit(deleteInstance);
			}
			return _instance;
		}

	   protected:
		TSingleton() {}
		virtual ~TSingleton() {}

		static void deleteInstance() {
			if (_instance != NULL)
				delete _instance;
			_instance = NULL;
		}

	   private:
		static T* _instance;
	};

	template <class T>
	T* TSingleton<T>::_instance = NULL;

}  // namespace utils

#endif
