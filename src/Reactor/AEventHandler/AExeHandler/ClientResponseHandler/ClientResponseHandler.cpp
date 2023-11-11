#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t& sharedData, va_list args)
		: AExeHandler(sharedData),
		  _request(*va_arg(args, request_t*)),
		  _director(this->chooseBuilder()),
		  _serverConfig(ServerManager::getInstance()->getServerConfig(this->getHandle())) {
		Dispatcher::getInstance()->registerIOHandler<ClientWriteHandlerFactory>(this->_sharedData);
		va_end(args);
	}
	ClientResponseHandler::~ClientResponseHandler() {}

	void ClientResponseHandler::handleEvent() {
		if (this->removeHandlerIfNecessary())
			return;
		try {
			if (this->_director.buildProduct() == false)  // may be throw ErrorReponseBuilder
				return;
			if (this->_director.getBuilderReadState() == RESOLVE && this->getBuffer().empty()) {
				Dispatcher::getInstance()->removeIOHandler(this->getHandle(), this->getType());
				Dispatcher::getInstance()->removeExeHandler(this);
				this->setState(RESOLVE);
			}
		} catch (const utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			// catch ErrorResponseBuilder
			// and set ErrorResponseBuilder in director
			this->_director.setBuilder(e);
		}
	}

	utils::shared_ptr<IBuilder<sharedData_t> > ClientResponseHandler::chooseBuilder() {
		try {
			// if (this->_request.get()->first == ERROR)
				// throw ErrorResponseBuilder(utils::itos(this->_request.get()->second.getErrorCode()))));
				
			std::vector<enum HttpMethods> methods = this->_serverConfig.get()->getDirectives(LIMIT_EXCEPT).asMedVec();

			// if (std::find(methods.begin(), methods.end(), this->_request.get()->second.getMethod()) == methods.end())
			// 	throw ErrorResponseBuilder(utils::itos(405));


			if (this->_request.get()->second.getMethod() == GET)
				return utils::shared_ptr<IBuilder<sharedData_t> >(
					new GetResponseBuilder(this->_sharedData, this->_request, this->_serverConfig));
		} catch (const utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			// catch ErrorResponseBuilder
			// and set ErrorResponseBuilder in director
			this->_director.setBuilder(e);
		} catch (const std::exception& e) {
			// this->_director.setBuilder(utils::shared_ptr<IBuilder<sharedData_t>()); // error 500 internal server error
		}
		return utils::shared_ptr<IBuilder<sharedData_t> >();
	}

}  // namespace reactor
