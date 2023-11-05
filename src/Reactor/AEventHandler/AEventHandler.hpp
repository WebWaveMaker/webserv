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
		handle_t getHandle() const { return this->_sharedData.get()->fd; };
		std::vector<char>& getBuffer() { return this->_sharedData.get()->buffer; };
		enum EventType getType() const { return this->_sharedData.get()->type; };
		enum AsyncState getState() const { return this->_sharedData.get()->state; };
		void setState(const enum AsyncState state) { this->_sharedData.get()->state = state; };
		virtual void handleEvent() = 0;
	};

}  // namespace reactor

#endif
