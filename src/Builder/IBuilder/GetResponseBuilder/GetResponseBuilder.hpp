#pragma once
#ifndef GETRESPONSEBUILDER_HPP
#define GETRESPONSEBUILDER_HPP

#include "GetResponseBuilder.h"

class GetResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	reactor::sharedData_t _sharedData;
	const request_t _request;
	const utils::shared_ptr<ServerConfig> _serverConfig;
	const utils::shared_ptr<LocationConfig> _locationConfig;  // may be needed?
	bool _removed;

	std::string _path;
	fd_t _fd;

	reactor::sharedData_t _readSharedData;	// for file, pipe read
	HttpMessage _response;	// for startLine, headers // defaultResponseBuilder가 기본적인 것들을 채울 예정.

	fd_t findReadFile();

	fd_t fileProcessing();

	void makeListHtml(const std::string& path, const std::vector<std::string>& dirVec);
	std::vector<std::string> readDir(const std::string& path);
	fd_t directoryListing();

	fd_t directoryProcessing();

   public:
	GetResponseBuilder(reactor::sharedData_t sharedData, request_t request,
					   const utils::shared_ptr<ServerConfig>& serverConfig,
					   const utils::shared_ptr<LocationConfig>& locationConfig);
	~GetResponseBuilder();

	// fileread RESOLVE, write empty
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