#pragma once
#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "HttpMessage.h"

class HttpMessage {
   private:
	std::vector<std::string> _startLine;
	std::map<std::string, std::string> _headers;
	std::string _body;
	int _errorCode;
	unsigned int _contentLength;
	unsigned int _contentLengthReceived;
	unsigned int _totalChunkedLength;
	std::string _buf;
	bool _isRegistered;

   public:
	HttpMessage();
	HttpMessage(const HttpMessage& obj);
	~HttpMessage();
	HttpMessage& operator=(const HttpMessage& obj);

	void setStartLine(const std::vector<std::string> startLine);
	void setErrorCode(const int errorCode);
	int getErrorCode(void) const;
	void setHeaders(const std::map<std::string, std::string>& headers);
	void setBody(const std::string& body);
	void setChunkedBody(const std::string& body);
	std::string combineHeaders(void);
	enum HttpMethods getMethod(void) const;
	std::string getMethodStr(void) const;
	std::string getRequestTarget(void) const;
	void setRequestTarget(const std::string& requestTarget);
	std::string getTargetFile(void) const;
	std::string getTargetPath(void) const;
	std::string& getBody(void);
	std::map<std::string, std::string>& getHeaders(void);
	std::string getRawStr(void);
	unsigned int getContentLength(void) const;
	void setContentLength(const unsigned int contentLength);
	unsigned int getContentLengthReceived(void) const;
	void setContentLengthReceived(const unsigned int contentLengthReceived);
	void reset();
	void setBuf(const std::string& buf);
	std::string& getBuf(void);
	void setTotalChunkedLength(const unsigned int totalChunkedLength);
	unsigned int getTotalChunkedLength(void) const;
	void setIsRegistered(const bool isRegistered);
	bool getIsRegistered(void) const;
	bool isSession(void) const;
	std::string getSessionId(void) const;
};

typedef utils::shared_ptr<HttpMessage> msg_t;

#endif
