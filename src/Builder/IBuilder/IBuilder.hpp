#pragma once
#ifndef IBUILDER_HPP
#define IBUILDER_HPP

#include "IBuilder.h"

template <typename T>
class IBuilder {
   protected:
	IBuilder() {}

	enum FileMode checkFileMode(const std::string& path) {
		struct stat fileStat;

		if (stat(path.c_str(), &fileStat) == -1)
			return MODE_ERROR;
		if (S_ISREG(fileStat.st_mode))
			return MODE_FILE;
		if (S_ISDIR(fileStat.st_mode))
			return MODE_DIRECTORY;
		return MODE_ERROR;
	}

   public:
	virtual ~IBuilder(){};
	virtual enum AsyncState getReadState() const = 0;
	virtual void setReadState(enum AsyncState state) = 0;
	virtual T getProduct() = 0;
	virtual void setStartLine() = 0;
	virtual void setHeader() = 0;
	virtual bool setBody() = 0;
	virtual void reset() = 0;
	virtual bool build() = 0;
	virtual void prepare() = 0;
};

#endif