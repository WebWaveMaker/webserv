#pragma once
#ifndef FILEREADHANDLERFACTORY_HPP
#define FILEREADHANDLERFACTORY_HPP

#include "FileReadHandlerFactory.h"

class FileReadHandlerFactory : public IOhandlerFactory {
   private:
   public:
	FileReadHandlerFactory();
	~FileReadHandlerFactory();
    virtual utils::shared_ptr<AEventHandler> createHandler(sharedData_t& sharedData);
};

#endif

