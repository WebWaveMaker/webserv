#pragma once
#ifndef IHTTPPARSER_HPP
#define IHTTPPARSER_HPP

class IHttpParser {
   private:
   public:
	IHttpParser();
	IHttpParser(const IHttpParser& obj);
	~IHttpParser();
	IHttpParser& operator=(const IHttpParser& obj);
};

#endif
