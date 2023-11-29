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
		unsigned int _readByte;
		request_t _request;

	   public:
		SharedData(const fd_t fd, const enum EventType type, std::vector<char> buffer);
		SharedData(const fd_t fd, const enum EventType type, std::vector<char> buffer, request_t request);
		SharedData(const SharedData& obj);
		~SharedData();
		SharedData& operator=(const SharedData& obj);

		fd_t getFd() const { return this->_fd; };
		void setFd(const fd_t fd) { const_cast<fd_t&>(this->_fd) = fd; };
		enum EventType getType() const { return this->_type; };
		std::vector<char>& getBuffer() { return this->_buffer; };
		enum AsyncState getState() const { return this->_state; };
		void setState(const enum AsyncState state) { this->_state = state; };
		void clear() {
			this->_buffer.clear();
			this->_state = PENDING;
		};
		unsigned int getReadByte() const { return this->_readByte; };
		void setReadByte(const unsigned int readByte) { this->_readByte = readByte; };
		request_t getRequest() const { return this->_request; };
		void setRequest(const request_t request) { this->_request = request; };
	};

	typedef utils::shared_ptr<SharedData> sharedData_t;
};	// namespace reactor

#endif
