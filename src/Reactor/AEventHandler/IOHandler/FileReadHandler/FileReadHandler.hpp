#pragma once
#ifndef FILEREADHANDLER_HPP
#define FILEREADHANDLER_HPP

#include "FileReadHandler.h"

namespace reactor {
	class FileReadHandler : public AEventHandler {
	   private:
		FILE* _fp;
		fd_t _fd;

	   public:
		FileReadHandler(sharedData_t& sharedData);
		virtual ~FileReadHandler();
		virtual void handleEvent();
	};

}  // namespace reactor

#endif
