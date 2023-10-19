#pragma once
#ifndef IEVENTHANDLER_HPP
#define IEVENTHANDLER_HPP

#include "IEventHandler.h"

namespace reactor {

class IEventHandler {
   private:
   protected:
   public:
	virtual ~IEventHandler();
	virtual handle_t getHandle() const = 0;
	virtual void handleRead() = 0;
	virtual void handleWrite() = 0;
	virtual void handleError() = 0;
};

}  // namespace reactor
#endif
