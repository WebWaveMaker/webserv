#pragma once
#ifndef AEVENTHANDLER_HPP
#define AEVENTHANDLER_HPP

#include "AEventHandler.h"

namespace reactor {

	class AEventHandler {
	   private:
	   protected:
		sharedData_t _sharedData;

	   public:
		AEventHandler(sharedData_t& sharedData) : _sharedData(sharedData){};
		virtual ~AEventHandler(){};
		sharedData_t getData() const { return this->_sharedData; };
		handle_t getHandle() const { return this->_sharedData->getFd(); };
		std::vector<char>& getBuffer() { return this->_sharedData->getBuffer(); };
		enum EventType getType() const { return this->_sharedData->getType(); };
		enum AsyncState getState() const { return this->_sharedData->getState(); };
		void setState(const enum AsyncState state) { this->_sharedData->setState(state); };
		void setHandler(const handle_t handle) { this->_sharedData->setFd(handle); };
		unsigned int getReadByte() const { return this->_sharedData->getReadByte(); };
		void setReadByte(const unsigned int readByte) { this->_sharedData->setReadByte(readByte); };
		virtual void handleEvent() = 0;
	};

}  // namespace reactor

#endif
