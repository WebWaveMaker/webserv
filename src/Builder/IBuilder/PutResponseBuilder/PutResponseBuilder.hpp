#pragma once
#ifndef PUT_RESPONSE_BUILDER_HPP
#define PUT_RESPONSE_BUILDER_HPP

#include "PutResponseBuilder.h"

class PutResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	reactor::sharedData_t _sharedData;
	const request_t _request;
	const utils::shared_ptr<ServerConfig> _serverConfig;
	const utils::shared_ptr<LocationConfig> _locationConfig;
	utils::shared_ptr<reactor::SharedData> _writeSharedData;
	bool _isExist;
	bool _isRemoved;
	SessionData* _sessionData;

	HttpMessage _response;
	std::string _path;
	fd_t _fd;

	void setPath(const std::string& target, const std::string targetPath);

   public:
	PutResponseBuilder(reactor::sharedData_t sharedData, request_t request,
					   const utils::shared_ptr<ServerConfig>& serverConfig,
					   const utils::shared_ptr<LocationConfig>& locationConfig, SessionData* sessionData);
	~PutResponseBuilder();

	static utils::shared_ptr<IBuilder<reactor::sharedData_t> > createPutResponseBuilder(
		const reactor::sharedData_t& sharedData, const request_t& request,
		const utils::shared_ptr<ServerConfig>& serverConfig, const utils::shared_ptr<LocationConfig>& locationConfig,
		SessionData* sessionData);

	virtual enum AsyncState getReadState() const { return this->_writeSharedData->getState(); }
	virtual void setReadState(enum AsyncState state) { this->_writeSharedData->setState(state); }
	virtual reactor::sharedData_t getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual bool build();
	virtual void prepare();
};

#endif