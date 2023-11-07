#include "AExeHandler.hpp"
#include "Dispatcher.hpp"

namespace reactor {
	bool AExeHandler::removeHandlerIfNecessary() {
		Dispatcher* dispatcher = Dispatcher::getInstance();

		if (dispatcher->isFdMarkedToClose(this->getHandle())) {
			dispatcher->removeExeHandler(this);
			return true;  // Indicating that the handler was removed
		}

		enum AsyncState state = this->getState();
		if (state == RESOLVE) {
			dispatcher->removeIOHandler(this->getHandle(), this->getType());
			return false;  // Continue execution
		} else if (state == TERMINATE) {
			dispatcher->addFdToClose(this->getHandle());
			dispatcher->removeExeHandler(this);
			return true;  // Indicating that the handler was removed
		}

		// Continue execution if none of the conditions were met
		return false;
	}
}  // namespace reactor
