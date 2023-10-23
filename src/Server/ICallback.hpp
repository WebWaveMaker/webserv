#pragma once
#ifndef ICALLBACK_HPP
#define ICALLBACK_HPP

#include "ICallback.h"

class ICallback {
   private:
	ICallback& operator=(const ICallback& obj);
	ICallback(const ICallback& obj);
	ICallback();

   public:
	virtual void execute() = 0;
	virtual ~ICallback();
};

#endif