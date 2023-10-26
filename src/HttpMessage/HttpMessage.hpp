#pragma once
#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "HttpMessage.h"

class HttpMessage {
   private:
   public:
	HttpMessage();
	HttpMessage(const HttpMessage& obj);
	~HttpMessage();
	HttpMessage& operator=(const HttpMessage& obj);
};

#endif
