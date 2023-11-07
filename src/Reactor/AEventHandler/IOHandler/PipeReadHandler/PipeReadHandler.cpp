#include "PipeReadHandler.hpp"

namespace reactor {
	PipeReadHandler::PipeReadHandler(sharedData_t& sharedData) : AEventHandler(sharedData) {}
	PipeReadHandler::~PipeReadHandler() {}
	void PipeReadHandler::handleEvent() {
        if (this->getState() == TERMINATE)
            return ;
        std::vector<char> buffer(BUFFER_SIZE);
        
        

    }

}  // namespace reactor