#pragma once
#ifndef DELETE_RESPONSE_BUILDER_HPP
#define DELETE_RESPONSE_BUILDER_HPP

#include "DeleteResponseBuilder.h"

class DeleteResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	reactor::sharedData_t _sharedData;
	const request_t _request;
	const utils::shared_ptr<ServerConfig> _serverConfig;
	const utils::shared_ptr<LocationConfig> _locationConfig;

	HttpMessage _response;
	std::string _path;

   public:
	DeleteResponseBuilder(reactor::sharedData_t sharedData, request_t request,
						  const utils::shared_ptr<ServerConfig>& serverConfig,
						  const utils::shared_ptr<LocationConfig>& locationConfig);
	~DeleteResponseBuilder();

	std::vector<std::string> split(const std::string& str, char delim);
	std::string mergePaths(const std::string& serverRoot, const std::string& reqPath);
	void deleteFile();
	virtual enum AsyncState getReadState() const { return this->_sharedData.get()->getState(); }
	virtual void setReadState(enum AsyncState state) { this->_sharedData.get()->setState(state); }
	virtual reactor::sharedData_t getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual bool build();
	virtual void prepare();
};

#endif