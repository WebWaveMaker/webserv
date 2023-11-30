#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t& sharedData)
		: AExeHandler(sharedData),
		  _request(sharedData->getRequest()),
		  _serverConfig(ServerManager::getInstance()->getServerConfig(this->getHandle())),
		  _locationConfig(_serverConfig->getLocationConfig(this->findLocationBlock())),
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

	std::string ClientResponseHandler::findLocationBlock() {
		std::string requestTarget = this->_request->second.getRequestTarget();

		size_t dotPos = requestTarget.find('.');
		if (dotPos == std::string::npos)
			return requestTarget;
		size_t slashPos = requestTarget.find('/', dotPos);
		if (slashPos == std::string::npos) {
			requestTarget.erase(0, dotPos);
			if (this->_serverConfig->getLocation("/" + requestTarget + "/").get() == NULL)
				return this->_request->second.getRequestTarget();
			std::vector<HttpMethods> methods =
				this->_serverConfig->getLocation("/" + requestTarget + "/")->getDirectives(LIMIT_EXCEPT).asMedVec();
			for (std::vector<HttpMethods>::iterator it = methods.begin(); it != methods.end(); ++it) {
				if (*it == this->_request->second.getMethod())
					return "/" + requestTarget;
			}
			return this->_request->second.getRequestTarget();
		}
		requestTarget.erase(slashPos);
		requestTarget.erase(0, dotPos);
		if (this->_serverConfig->getLocation("/" + requestTarget + "/").get() == NULL)
			return this->_request->second.getRequestTarget();
		std::vector<HttpMethods> methods =
			this->_serverConfig->getLocation("/" + requestTarget + "/")->getDirectives(LIMIT_EXCEPT).asMedVec();
		for (std::vector<HttpMethods>::iterator it = methods.begin(); it != methods.end(); ++it) {
			if (*it == this->_request->second.getMethod())
				return "/" + requestTarget;
		}
		return this->_request->second.getRequestTarget();
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
		// std::cout << "client response handler" << std::endl;
		try {
			if (this->getBuffer().empty() && this->_director.getProduct()->getBuffer().empty() &&
				this->_director.getBuilderReadState() == RESOLVE) {
				Dispatcher::getInstance()->removeIOHandler(this->getHandle(), this->getType());
				Dispatcher::getInstance()->removeExeHandler(this);
				if (this->_keepalive == false) {
					this->_sharedData->setState(TERMINATE);
					Dispatcher::getInstance()->addFdToClose(this->getHandle());
				}
				// std::cout << "remove responseHandler" << std::endl;
				return;
			}
			// std::cout << "this one?" << std::endl;
			if (this->_director.buildProduct() == false) {
				return;
			}
			// std::cout << "client response handler try catch" << std::endl;
		} catch (utils::shared_ptr<IBuilder<sharedData_t> >& builder) {
			this->_director.setBuilder(builder);
		} catch (...) {
			// build 도중 에러가 발생하면 ClientWriteHandler와 자신을 삭제하고 clientFd를 연결종료에 등록합니다.
			// builder는 에러르 throw하기전에 자신이 사용중이던 handler와 자원들을 적절히 정리하고 throw해야합니다.
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
			if (_locationConfig.get() == u::nullptr_t)
				throw ErrorResponseBuilder::createErrorResponseBuilder(NOT_FOUND, this->_sharedData,
																	   this->_serverConfig, this->_locationConfig);
			if (this->_request->first == HTTP_ERROR)
				throw ErrorResponseBuilder::createErrorResponseBuilder(this->_request->second.getErrorCode(),
																	   this->_sharedData, this->_serverConfig,
																	   this->_locationConfig);

			const std::vector<enum HttpMethods> methods = this->_locationConfig->getDirectives(LIMIT_EXCEPT).asMedVec();

			if (std::find(methods.begin(), methods.end(), this->_request->second.getMethod()) == methods.end())
				throw ErrorResponseBuilder::createErrorResponseBuilder(METHOD_NOT_ALLOWED, this->_sharedData,
																	   this->_serverConfig, this->_locationConfig);
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
				return ErrorResponseBuilder::createErrorResponseBuilder(BAD_REQUEST, this->_sharedData,
																		this->_serverConfig, this->_locationConfig);
			}
		} catch (utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			return e;
		} catch (...) {
			return ErrorResponseBuilder::createErrorResponseBuilder(INTERNAL_SERVER_ERROR, this->_sharedData,
																	this->_serverConfig, this->_locationConfig);
		}
		return ErrorResponseBuilder::createErrorResponseBuilder(INTERNAL_SERVER_ERROR, this->_sharedData,
																this->_serverConfig, this->_locationConfig);
	}
}  // namespace reactor
