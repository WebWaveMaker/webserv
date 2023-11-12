#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t& sharedData, va_list args)
		: AExeHandler(sharedData),
		  _request(*va_arg(args, request_t*)),
		  _serverConfig(ServerManager::getInstance()->getServerConfig(this->getHandle())),
		  _locationConfig(_serverConfig.get()->getLocationConfig(_request.get()->second.getRequestTarget())),
		  _director(this->chooseBuilder()) {
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
			this->_director.setBuilder(e);
		}
	}

	utils::shared_ptr<IBuilder<sharedData_t> > ClientResponseHandler::chooseBuilder() {
		try {
			// if (this->_request.get()->first == ERROR)
			this->_request.get()->second.setErrorCode(404);
				throw utils::shared_ptr<IBuilder<sharedData_t> >(
					new ErrorResponseBuilder(this->_request.get()->second.getErrorCode(), this->_sharedData,
											 this->_serverConfig, this->_locationConfig));
			// std::vector<enum HttpMethods> methods = this->_serverConfig.get()->getDirectives(LIMIT_EXCEPT).asMedVec();
			// if (std::find(methods.begin(), methods.end(), this->_request.get()->second.getMethod()) == methods.end())
			// 	throw utils::shared_ptr<IBuilder<sharedData_t> >(
					// new ErrorResponseBuilder(405, this->_sharedData, this->_serverConfig, this->_locationConfig));
			if (this->_request.get()->second.getMethod() == GET)
				return utils::shared_ptr<IBuilder<sharedData_t> >(new GetResponseBuilder(
					this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
		} catch (utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			this->_director.setBuilder(e);
		} catch (...) {
			this->_director.setBuilder(utils::shared_ptr<IBuilder<sharedData_t> >(
				new ErrorResponseBuilder(500, this->_sharedData, this->_serverConfig, this->_locationConfig)));
		}
		return utils::shared_ptr<IBuilder<sharedData_t> >();
	}

}  // namespace reactor
