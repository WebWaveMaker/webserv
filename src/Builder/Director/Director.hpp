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

	enum AsyncState getBuilderReadState() const {
		if (this->_builder.get() == utils::nullptr_t)
			return NONE;
		return this->_builder->getReadState();
	}

	void setBuilderReadState(enum AsyncState state) {
		if (this->_builder.get() == utils::nullptr_t)
			return;
		this->_builder->setReadState(state);
	}

	bool buildProduct() {
		if (this->_builder.get() == utils::nullptr_t)
			return false;
		return this->_builder->build();
	};

	T getProduct() { return this->_builder->getProduct(); };

	void setBuilder(utils::shared_ptr<IBuilder<T> > builder) { this->_builder = builder; };

	utils::shared_ptr<IBuilder<T> > getBuilder(void) const { return this->_builder; }
};

#endif
