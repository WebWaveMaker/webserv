#pragma once
#ifndef MIME_HPP
#define MIME_HPP

#include "Mime.h"

class Mime {
   private:
	std::map<std::string, std::string> _mimeTypes;

   public:
	Mime();
	Mime(const Mime& other);
	~Mime();
	Mime& operator=(const Mime& other);

	void setMimeTypes(std::string extension, std::string mimeType);
	std::string getMimeType(const std::string& extension) const;
};

#endif	// MIME_HPP