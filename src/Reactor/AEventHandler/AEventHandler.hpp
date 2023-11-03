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
		AEventHandler(sharedData_t sharedData) : _sharedData(sharedData){};
		virtual ~AEventHandler(){};
		virtual sharedData_t getData() const { return this->_sharedData; };
		virtual handle_t getHandle() const { return this->_sharedData.get()->fd; };
		virtual std::vector<char> getBuffer() const { return this->_sharedData.get()->buffer; };
		virtual enum ASyncState getState() const { return this->_sharedData.get()->state; };
		virtual void setState(const enum AsyncState state) { this->_sharedData.get()->state = state; };
		virtual void handleEvent() = 0;
	};

}  // namespace reactor

#endif
