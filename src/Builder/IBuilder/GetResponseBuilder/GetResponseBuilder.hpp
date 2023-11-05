#pragma once
#ifndef GETRESPONSEBUILDER_HPP
#define GETRESPONSEBUILDER_HPP

#include "GetResponseBuilder.h"

class GetResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	utils::shared_ptr<ServerConfig> _config;
	std::string _filedata;
	reactor::sharedData_t _response;

   public:
	GetResponseBuilder(utils::shared_ptr<ServerConfig> config, reactor::sharedData_t _response, std::string filedata);
	~GetResponseBuilder();
	virtual reactor::sharedData_t getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual void build();
	virtual void prepare();
};

#endif