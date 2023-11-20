#include "FileWriteHandlerFactory.hpp"

namespace reactor {
	FileWriteHandlerFactory::FileWriteHandlerFactory() {}
	FileWriteHandlerFactory::~FileWriteHandlerFactory() {}

	utils::shared_ptr<AEventHandler> FileWriteHandlerFactory::createIOHandler(sharedData_t& sharedData) {
		return utils::static_pointer_cast<AEventHandler, FileWriteHandler>(
			utils::shared_ptr<FileWriteHandler>(new FileWriteHandler(sharedData)));
	}
}  // namespace reactor