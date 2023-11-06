#pragma once
#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP

#include "SharedData.h"

namespace reactor {
	class SharedData {
	   private:
		const fd_t _fd;
		const enum EventType _type;
		std::vector<char> _buffer;
		enum AsyncState _state;

	   public:
		SharedData(const fd_t fd, const enum EventType type, std::vector<char> buffer);
		SharedData(const SharedData& obj);
		~SharedData();
		SharedData& operator=(const SharedData& obj);

		fd_t getFd() const { return this->_fd; };
		enum EventType getType() const { return this->_type; };
		std::vector<char>& getBuffer() { return this->_buffer; };
		enum AsyncState getState() const { return this->_state; };
		void setState(const enum AsyncState state) { this->_state = state; };
	};

	typedef utils::shared_ptr<SharedData> sharedData_t;
};	// namespace reactor

#endif
