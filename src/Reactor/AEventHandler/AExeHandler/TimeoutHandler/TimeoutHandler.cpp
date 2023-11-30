#include "TimeoutHandler.hpp"
#include "Dispatcher.hpp"

namespace reactor {
	TimeoutHandler::TimeoutHandler(sharedData_t& sharedData) : AExeHandler(sharedData) {
		Dispatcher::getInstance()->registerIOHandler<TimerHandlerFactory>(sharedData);
	}

	void TimeoutHandler::handleEvent() {
		if (this->getState() != ACCEPT)
			return;
		ServerManager* serverManager = ServerManager::getInstance();
		u::shared_ptr<std::vector<fd_t> > clientFds = serverManager->getClientFds();
		SyncEventDemultiplexer* demultiplexer = SyncEventDemultiplexer::getInstance();
		for (std::vector<fd_t>::size_type i = 0; i < clientFds->size(); ++i) {
			std::time_t curTime = std::time(NULL);
			std::time_t fdTime = demultiplexer->getFdTime((*clientFds.get())[i]);
			unsigned int keepAliveTime = ServerManager::getInstance()
											 ->getServerConfig((*clientFds.get())[i])
											 ->getDirectives(KEEPALIVE_TIMEOUT)
											 .asUint();
			if (std::difftime(curTime, fdTime) >= keepAliveTime) {
				if (Dispatcher::getInstance()->isWriting((*clientFds.get())[i]))
					continue;
				Dispatcher::getInstance()->addFdToClose((*clientFds.get())[i]);
			}
		}
		this->setState(PENDING);
	}

	TimeoutHandler::~TimeoutHandler() {}
}  // namespace reactor
