#pragma once
#ifndef CGIRESPONSEBUILDER_HPP
#define CGIRESPONSEBUILDER_HPP

#include "CgiResponseBuilder.h"

// sharedData의 fd는 클라이언트의 fd 이다.

class CgiResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	reactor::sharedData_t _sharedData;
	const request_t _request;
	const utils::shared_ptr<ServerConfig> _serverConfig;
	const utils::shared_ptr<LocationConfig> _locationConfig;
	std::string _cgiFullPath;
	std::map<std::string, std::string> _interpreterMap;
	int _sv[2];

	reactor::sharedData_t _cgiWriteSharedData;
	reactor::sharedData_t _cgiReadSharedData;
	HttpMessage _response;
	std::vector<std::string> _startLine;
	bool _startLineState;

	CgiResponseBuilder(const CgiResponseBuilder& obj);
	CgiResponseBuilder& operator=(const CgiResponseBuilder& obj);

   public:
	CgiResponseBuilder(reactor::sharedData_t sharedData, request_t request,
					   const utils::shared_ptr<ServerConfig>& serverConfig,
					   const utils::shared_ptr<LocationConfig>& locationConfig);
	~CgiResponseBuilder();

	virtual enum AsyncState getReadState() const { return this->_cgiReadSharedData.get()->getState(); }
	virtual void setReadState(enum AsyncState state) { this->_cgiReadSharedData.get()->setState(state); }
	virtual reactor::sharedData_t getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual bool build();
	virtual void prepare();

	std::string makeCgiFullPath();
	bool makeSocketPair();
	void makeWriteSharedData();
	bool doFork();
	char** setEnvp();
	std::string makeQueryString();
	std::string makePathInfo();
	char** makeArgs();
	std::string makeInterpreter();
	std::string makeExtension();
	std::vector<std::string> parsPathEnvp();
	void inItInterpreterMap();
	void replaceStartLine();
	void cgiStartLineInsert();
	void addCgiEnvp(std::vector<std::string>& cgiEnvpVec, const std::string& key, const std::string& value);
};

// Start Line: GET / HTTP/1.1
// Headers:
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
// Accept-Encoding: gzip, deflate
// Accept-Language: en-us
// Connection: keep-alive
// Host: localhost:4240
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.6.1 Safari/605.1.15
// Body:
// Error Code: 0

#endif
