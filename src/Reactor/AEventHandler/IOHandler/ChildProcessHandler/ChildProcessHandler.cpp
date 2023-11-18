#include "ChildProcessHandler.hpp"

namespace reactor {
	ChildProcessHandler::ChildProcessHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}

	ChildProcessHandler::~ChildProcessHandler() {}

	void ChildProcessHandler::handleEvent() {
		int status;
		waitpid(this->getHandle(), &status, );
	}
}  // namespace reactor
