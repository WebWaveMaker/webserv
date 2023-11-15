#pragma once
#ifndef REDIRECTRESPONSEBUILDER_HPP
#define REDIRECTRESPONSEBUILDER_HPP

#include "RedirectResponseBuilder.h"

class RedirectResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
   public:
	RedirectResponseBuilder(const int statusCode, reactor::sharedData_t sharedData,
							const utils::shared_ptr<ServerConfig>& serverConfig,
							const utils::shared_ptr<LocationConfig>& locationConfig);
	~RedirectResponseBuilder();
	virtual enum AsyncState getReadState() const;
	virtual void setReadState(enum AsyncState state);
	virtual T getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual bool build();
	virtual void prepare();
};

#endif
