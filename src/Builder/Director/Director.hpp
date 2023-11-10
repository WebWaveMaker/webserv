#pragma once
#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include "Director.h"

template <typename T>
class Director {
   private:
	utils::shared_ptr<IBuilder<T> > _builder;

   public:
	Director() {}
	Director(utils::shared_ptr<IBuilder<T> > builder) : _builder(builder) {}
	~Director() {}
	bool buildProduct() { this->_builder.get()->build(); };
	void setBuilder(utils::shared_ptr<IBuilder<T> > builder) { this->_builder = builder; };
	utils::shared_ptr<IBuilder<T> > getBuilder(void) const { return this->_builder; }
};

#endif
