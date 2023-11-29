#include "FileReadHandlerFactory.hpp"

namespace reactor {
	FileReadHandlerFactory::FileReadHandlerFactory() {}
	FileReadHandlerFactory::~FileReadHandlerFactory() {}

	utils::shared_ptr<AEventHandler> FileReadHandlerFactory::createIOHandler(sharedData_t& sharedData) {
		return utils::static_pointer_cast<AEventHandler, FileReadHandler>(
			utils::shared_ptr<FileReadHandler>(new FileReadHandler(sharedData)));
	}
}  // namespace reactor