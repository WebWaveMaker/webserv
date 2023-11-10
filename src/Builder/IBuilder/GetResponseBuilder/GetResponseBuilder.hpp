#pragma once
#ifndef GETRESPONSEBUILDER_HPP
#define GETRESPONSEBUILDER_HPP

#include "GetResponseBuilder.h"

class GetResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	reactor::sharedData_t _sharedData;
	const request_t _request;
	const utils::shared_ptr<ServerConfig>& _serverConfig;
	// utils::shared_ptr<LocationConfig>& _locationConfig; // may be needed?

	reactor::sharedData_t _readSharedData;	// for file, pipe read
	HttpMessage _response;	// for startLine, headers // defaultResponseBuilder가 기본적인 것들을 채울 예정.

   public:
	GetResponseBuilder(reactor::sharedData_t sharedData, request_t request, const utils::shared_ptr<ServerConfig>& config);
	~GetResponseBuilder();
	virtual reactor::sharedData_t getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual bool build();
	virtual void prepare();
};

#endif