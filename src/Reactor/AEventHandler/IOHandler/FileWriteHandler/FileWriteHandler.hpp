#pragma once
#ifndef FILEWRITEHANDLER_HPP
#define FILEWRITEHANDLER_HPP

#include "FileWriteHandler.h"

namespace reactor {
	class FileWriteHandler : public AEventHandler {
	   private:
		FILE* _fp;

	   public:
		FileWriteHandler(sharedData_t& sharedData);
		virtual ~FileWriteHandler();
		virtual void handleEvent();
	};
}  // namespace reactor
#endif
