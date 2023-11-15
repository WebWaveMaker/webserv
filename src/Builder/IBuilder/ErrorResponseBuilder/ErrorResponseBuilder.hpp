#pragma once
#ifndef ERRORRESPONSEBUILDER_HPP
#define ERRORRESPONSEBUILDER_HPP

#include "ErrorResponseBuilder.h"

class ErrorResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	int _errorCode;
	reactor::sharedData_t _sharedData;
	reactor::sharedData_t _readSharedData;	// for file read;
	const utils::shared_ptr<ServerConfig> _serverConfig;
	const utils::shared_ptr<LocationConfig> _locationConfig;

	std::string _path;
	fd_t _fd;

	HttpMessage _response;

	fd_t findReadFile();

   public:
	ErrorResponseBuilder(const int errorCode, reactor::sharedData_t sharedData,
						 const utils::shared_ptr<ServerConfig>& config,
						 const utils::shared_ptr<LocationConfig>& locationConfig);
	~ErrorResponseBuilder();

	virtual enum AsyncState getReadState() const { return this->_readSharedData->getState(); }
	virtual void setReadState(enum AsyncState state) { this->_readSharedData->setState(state); }
	virtual reactor::sharedData_t getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual bool build();
	virtual void prepare();
};

#endif
