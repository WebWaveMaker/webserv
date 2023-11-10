#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t& sharedData, va_list args)
		: AExeHandler(sharedData), _request(*va_arg(args, request_t*)), _director(this->chooseBuilder()) {
		Dispatcher::getInstance()->registerIOHandler<ClientWriteHandlerFactory>(sharedData);
		va_end(args);
	}
	ClientResponseHandler::~ClientResponseHandler() {}

	void ClientResponseHandler::handleEvent() {
		if (this->removeHandlerIfNecessary())
			return;
		try {
			if (this->_director.buildProduct() == false)  // may be throw ErrorReponseBuilder
				return;
		} catch (const utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			// catch ErrorResponseBuilder
			// and set ErrorResponseBuilder in director
			this->_director.setBuilder(e);
		}
	}

	utils::shared_ptr<IBuilder<sharedData_t> > ClientResponseHandler::chooseBuilder() {
		try {
			if (this->_request.get()->second.getMethod() == GET)
				return utils::shared_ptr<IBuilder<sharedData_t> >(
					new GetResponseBuilder(this->_sharedData, this->_request,
										   ServerManager::getInstance()->getServerConfig(this->getHandle())));
		} catch (const utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			// catch ErrorResponseBuilder
			// and set ErrorResponseBuilder in director
			this->_director.setBuilder(e);
		}
		return utils::shared_ptr<IBuilder<sharedData_t> >();
	}

}  // namespace reactor
