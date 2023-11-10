#pragma once
#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "HttpMessage.h"

class HttpMessage {
   private:
	std::vector<std::string> _startLine;
	std::map<std::string, std::string> _headers;
	std::string _body;

   public:
	HttpMessage();
	HttpMessage(const HttpMessage& obj);
	~HttpMessage();
	HttpMessage& operator=(const HttpMessage& obj);

	void setStartLine(const std::vector<std::string> startLine);
	void setHeaders(const std::map<std::string, std::string>& headers);
	void setBody(const std::string& body);
	std::string combineHeaders(void);
	enum HttpMethods getMethod(void) const;
	std::string getRequestTarget(void) const;
	std::map<std::string, std::string>& getHeaders(void);
	std::string getRawStr(void);
	void reset();
};

typedef utils::shared_ptr<HttpMessage> msg_t;

#endif
