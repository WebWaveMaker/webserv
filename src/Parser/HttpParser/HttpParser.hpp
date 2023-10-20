#pragma once
#ifndef RequestParser_HPP
#define RequestParser_HPP

#include "RequestParser.h"

class RequestParser : public IParser<request_t> {
   private:
	std::queue<request_t> _msgs;
	request_t* _curMsg;
	std::string _buf;

	std::string findAndSubstr(std::string& buf, std::string delim);
	bool parseStartLine(std::string& buf);
	bool parseHeader(std::string& buf);
	bool parserBody(std::string& buf);
	RequestParser(const RequestParser& obj);
	RequestParser& operator=(const RequestParser& obj);
	request_t pop(void);

   public:
	RequestParser();
	virtual request_t parse(const std::string& content);
	virtual ~RequestParser();
	enum RequestParserState getState(void) const;
};

#endif
