#pragma once
#ifndef FILEREADHANDLERFACTORY_HPP
#define FILEREADHANDLERFACTORY_HPP

#include "FileReadHandlerFactory.h"

namespace reactor {
	class FileReadHandlerFactory : public IOhandlerFactory {
	   private:
	   public:
		FileReadHandlerFactory();
		~FileReadHandlerFactory();
		virtual utils::shared_ptr<AEventHandler> createIOHandler(sharedData_t& sharedData);
	};
}  // namespace reactor
#endif
