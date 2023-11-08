#pragma once
#ifndef FILEWRITEHANDLERFACTORY_HPP
#define FILEWRITEHANDLERFACTORY_HPP

#include "FileWriteHandlerFactory.h"

namespace reactor {
	class FileWriteHandlerFactory : public IOhandlerFactory {
	   private:
	   public:
		FileWriteHandlerFactory();
		~FileWriteHandlerFactory();
		utils::shared_ptr<AEventHandler> createHandler(sharedData_t& sharedData);
	};
}  // namespace reactor
#endif