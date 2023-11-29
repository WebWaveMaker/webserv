#pragma once
#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "RequestParser.h"

class RequestParser : public IParser<request_t, std::string> {
   private:
	std::queue<request_t> _msgs;
	request_t* _curMsg;
	utils::shared_ptr<ServerConfig> _serverConfig;

	void branchParser(const enum HttpMessageState state, std::string& buf);
	bool setErrorRequest(const enum HttpMessageState state, const enum HTTP_STATUS code);
	std::string findAndSubstr(std::string& buf, std::string delim);
	bool parseStartLine(std::string& buf);
	bool parseHeader(std::string& buf);
	bool parseBody(std::string& buf);
	bool parseLongBody(std::string& buf);
	bool checkContentLengthZero(const std::map<std::string, std::string>& headers);
	bool parseChunked(std::string& buf);
	RequestParser(const RequestParser& obj);
	request_t pop(void);
	HttpMessage& getCurMsg(void);
	inline void saveBufAndClear(std::string& buf);
	inline std::string cutBuf(std::string& buf, const std::string::size_type size);

   public:
	RequestParser(utils::shared_ptr<ServerConfig> serverConfig);
	virtual request_t parse(std::string& content);
	request_t get(void);
	virtual ~RequestParser();
	enum AsyncState getState(void) const;
};

#endif
