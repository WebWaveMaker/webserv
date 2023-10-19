#pragma once
#ifndef HTTPPARSERSTATE_HPP
#define HTTPPARSERSTATE_HPP

#include "HttpParserState.h"

class HttpParserState {
   private:
   public:
	HttpParserState();
	HttpParserState(const HttpParserState& obj);
	~HttpParserState();
	HttpParserState& operator=(const HttpParserState& obj);
};

#endif
