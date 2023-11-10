#pragma once
#ifndef IBUILDER_HPP
#define IBUILDER_HPP

#include "IBuilder.h"

template <typename T>
class IBuilder {
   protected:
	IBuilder() {}

   public:
	virtual ~IBuilder(){};
	virtual T getProduct() = 0;
	virtual void setStartLine() = 0;
	virtual void setHeader() = 0;
	virtual bool setBody() = 0;
	virtual void reset() = 0;
	virtual bool build() = 0;
	virtual void prepare() = 0;
};

#endif