#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t& sharedData)
		: AExeHandler(sharedData),
		  _request(sharedData->getRequest()),
		  _serverConfig(ServerManager::getInstance()->getServerConfig(this->getHandle(),
																	  this->_request->second.getHeaders()[HOST])),
		  _locationConfig(
			  _serverConfig.get() != u::nullptr_t
				  ? _serverConfig->getLocationConfig(ServerManager::findLocationBlock(this->_request, _serverConfig))
				  : u::shared_ptr<LocationConfig>()),
		  _keepalive(true),
		  _director(this->chooseBuilder()),
		  _registered(false) {
		std::cout << "[" << this->_request->second.getUserAgent() << " " << this->_request->second.getMethodStr() << " "
				  << this->_request->second.getRequestTarget() << "] "
				  << "Response start..." << std::endl;
	}

	ClientResponseHandler::~ClientResponseHandler() {
		std::cout << "[" << this->_request->second.getUserAgent() << " " << this->_request->second.getMethodStr() << " "
				  << this->_request->second.getRequestTarget() << "] "
				  << "Response end" << std::endl;
	}

	void ClientResponseHandler::handleEvent() {
		if (this->removeHandlerIfNecessary()) {
			this->_director.buildProduct();
			return;
		}
		if (this->_registered == false) {
			Dispatcher::getInstance()->registerIOHandler<ClientWriteHandlerFactory>(this->_sharedData);
			this->_registered = true;
		}
		try {
			if (this->getBuffer().empty() && this->_director.getProduct()->getBuffer().empty() &&
				this->_director.getBuilderReadState() == RESOLVE) {
				Dispatcher::getInstance()->removeIOHandler(this->getHandle(), this->getType());
				Dispatcher::getInstance()->removeExeHandler(this);
				if (this->_keepalive == false) {
					this->_sharedData->setState(TERMINATE);
					Dispatcher::getInstance()->addFdToClose(this->getHandle());
				}
				return;
			}
			if (this->_director.buildProduct() == false) {
				return;
			}
		} catch (utils::shared_ptr<IBuilder<sharedData_t> >& builder) {
			this->_director.setBuilder(builder);
		} catch (...) {
			this->setState(TERMINATE);
			this->_director.setBuilderReadState(TERMINATE);
			Dispatcher::getInstance()->removeIOHandler(this->getHandle(), this->getType());
			Dispatcher::getInstance()->removeExeHandler(this);
			Dispatcher::getInstance()->removeFdToClose(this->getHandle());
		}
	}

	utils::shared_ptr<IBuilder<sharedData_t> > ClientResponseHandler::chooseBuilder() {
		try {
			SessionData* sessionData =
				HttpSession::getInstance()->getSessionData(this->_request->second.getSessionId());
			if (this->_request->first == HTTP_ERROR)
				throw ErrorResponseBuilder::createErrorResponseBuilder(this->_request->second.getErrorCode(),
																	   this->_sharedData, this->_request,
																	   this->_serverConfig, this->_locationConfig);
			if (_locationConfig.get() == u::nullptr_t)
				throw ErrorResponseBuilder::createErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_request,
																	   this->_serverConfig, this->_locationConfig);
			if (this->_request->second.getHeaders()["Keep-Alive"] == "close")
				this->_keepalive = false;

			const std::vector<enum HttpMethods> methods = this->_locationConfig->getDirectives(LIMIT_EXCEPT).asMedVec();

			if (std::find(methods.begin(), methods.end(), this->_request->second.getMethod()) == methods.end())
				throw ErrorResponseBuilder::createErrorResponseBuilder(
					METHOD_NOT_ALLOWED, this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig);
			if (this->_locationConfig->isRedirect()) {
				std::vector<std::string> rv = this->_locationConfig->getDirectives(RETURN).asStrVec();

				throw RedirectResponseBuilder::createRedirectResponseBuilder(
					utils::stoui(rv[0]), rv[1], this->_sharedData, this->_request, this->_serverConfig);
			}
			if (this->_locationConfig->isCgi())
				return CgiResponseBuilder::createCgiResponseBuilder(
					this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig, sessionData);

			typedef utils::shared_ptr<IBuilder<sharedData_t> > (*ResponseBuilderFunction)(
				const sharedData_t&, const request_t&, const utils::shared_ptr<ServerConfig>&,
				const utils::shared_ptr<LocationConfig>&, SessionData*);
			ResponseBuilderFunction responseBuilderFunctions[] = {
				&GetResponseBuilder::createGetResponseBuilder, &HeadResponseBuilder::createHeadResponseBuilder,
				&PostResponseBuilder::createPostResponseBuilder, &DeleteResponseBuilder::createDeleteResponseBuilder,
				&PutResponseBuilder::createPutResponseBuilder};

			enum HttpMethods method = this->_request->second.getMethod();
			if (method >= 0 && method < sizeof(responseBuilderFunctions) / sizeof(responseBuilderFunctions[0])) {
				return (*responseBuilderFunctions[method])(this->_sharedData, this->_request, this->_serverConfig,
														   this->_locationConfig, sessionData);
			} else {
				return ErrorResponseBuilder::createErrorResponseBuilder(BAD_REQUEST, this->_sharedData, this->_request,
																		this->_serverConfig, this->_locationConfig);
			}
		} catch (utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			return e;
		} catch (...) {
			return ErrorResponseBuilder::createErrorResponseBuilder(
				INTERNAL_SERVER_ERROR, this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig);
		}
		return ErrorResponseBuilder::createErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig);
	}
}  // namespace reactor
