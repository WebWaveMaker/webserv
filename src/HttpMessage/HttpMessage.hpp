#pragma once
#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "HttpMessage.h"

class HttpMessage {
   private:
	std::string _startLine[3];
	std::map<std::string, std::string> _headers;
	std::string _body;

   public:
	HttpMessage();
	HttpMessage(const HttpMessage& obj);
	~HttpMessage();
	HttpMessage& operator=(const HttpMessage& obj);

	void setStartLine(const std::string startLine[3]);
	void setHeaders(const std::map<std::string, std::string>& headers);
	void setBody(const std::string& body);
	enum HttpMethods getMethod(void) const;
	std::map<std::string, std::string>& getHeaders(void);
	std::string getRawStr(void) const;
	void reset();
};

typedef utils::shared_ptr<HttpMessage> msg_t;

#endif
