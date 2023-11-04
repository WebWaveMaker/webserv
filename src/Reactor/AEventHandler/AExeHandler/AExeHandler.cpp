#include "AExeHandler.hpp"

bool AExeHandler::removeHandlerIfNecessary() {
	reactor::Dispatcher* dispatcher = reactor::Dispatcher::getInstance();

	if (dispatcher->isFdMarkedToClose(this->getHandle())) {
		dispatcher->removeExeHandler(u::shared_ptr<AEventHandler>(this));
		return true;  // Indicating that the handler was removed
	}

	HandlerState state = this->getState();
	if (state == DONE) {
		dispatcher->removeIOHandler(this->getHandle(), EVENT_READ);
		return false;  // Continue execution
	} else if (state == TERMINATE) {
		dispatcher->addFdToClose(this->getHandle());
		dispatcher->removeExeHandler(u::shared_ptr<AEventHandler>(this));
		return true;  // Indicating that the handler was removed
	}

	// Continue execution if none of the conditions were met
	return false;
}
