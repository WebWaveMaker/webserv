#pragma once
#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "RequestParser.h"

class RequestParser : public IParser<request_t> {
   private:
	std::queue<request_t> _msgs;
	request_t* _curMsg;
	std::string _buf;
	utils::shared_ptr<ServerConfig> _serverConfig;

	std::string findAndSubstr(std::string& buf, std::string delim);
	bool parseStartLine(std::string& buf);
	bool parseHeader(std::string& buf);
	bool parserBody(std::string& buf);
	bool checkContentLengthZero(const std::map<std::string, std::string> &headers);
	RequestParser(const RequestParser& obj);
	request_t pop(void);
	HttpMessage& getCurMsg(void);

   public:
	RequestParser(utils::shared_ptr<ServerConfig> serverConfig);
	virtual request_t parse(const std::string& content);
	request_t get(void);
	virtual ~RequestParser();
	enum AsyncState getState(void) const;
};

#endif
