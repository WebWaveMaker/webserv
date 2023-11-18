#pragma once
#ifndef POSTRESPONSEBUILDER_HPP
#define POSTRESPONSEBUILDER_HPP

#include "PostResponseBuilder.h"

// 여기에서 세션과 쿠키를 적용해야 겠다. 그리고 이걸로 회원가입과 로그인을 구현해야 한다. 
class PostResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	reactor::sharedData_t _sharedData;
	const request_t _request;
	const utils::shared_ptr<ServerConfig> _serverConfig;
	const utils::shared_ptr<LocationConfig> _locationConfig;
	HttpMessage _response;

	reactor::sharedData_t _readSharedData;

	void divideEntryPoint();

	class SignUp;
	class Login;

   public:
	PostResponseBuilder(reactor::sharedData_t sharedData, request_t request,
						const utils::shared_ptr<ServerConfig>& serverConfig,
						const utils::shared_ptr<LocationConfig>& locationConfig);
	~PostResponseBuilder();

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
