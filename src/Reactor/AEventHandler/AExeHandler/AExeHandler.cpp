#include "AExeHandler.hpp"
#include "Dispatcher.hpp"

bool AExeHandler::removeHandlerIfNecessary(enum EventType type) {
	reactor::Dispatcher* dispatcher = reactor::Dispatcher::getInstance();

	if (dispatcher->isFdMarkedToClose(this->getHandle())) {
		dispatcher->removeExeHandler(u::shared_ptr<AEventHandler>(this));
		return true;  // Indicating that the handler was removed
	}

	HandlerState state = this->getState();
	if (state == RESOLVE) {
		dispatcher->removeIOHandler(this->getHandle(), type);
		return false;  // Continue execution
	} else if (state == TERMINATE) {
		dispatcher->addFdToClose(this->getHandle());
		dispatcher->removeExeHandler(u::shared_ptr<AEventHandler>(this));
		return true;  // Indicating that the handler was removed
	}

	// Continue execution if none of the conditions were met
	return false;
}
