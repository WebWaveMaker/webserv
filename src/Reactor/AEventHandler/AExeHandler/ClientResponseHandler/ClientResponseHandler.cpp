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
		std::cerr << "make resesrioesjorijsoerijsoeijr" << std::endl;
	}

	ClientResponseHandler::~ClientResponseHandler() {
		std::cerr << "REsponse removeREsponse removeREsponse removeREsponse removeREsponse removeREsponse "
					 "removeREsponse removeREsponse remove"
				  << std::endl;
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
			if (this->_director.buildProduct() == false)
				return;
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

	static utils::shared_ptr<IBuilder<sharedData_t> > createGetResponseBuilder(
		const sharedData_t& sharedData, const request_t& request, const utils::shared_ptr<ServerConfig>& serverConfig,
		const utils::shared_ptr<LocationConfig>& locationConfig) {
		return utils::shared_ptr<IBuilder<sharedData_t> >(
			new GetResponseBuilder(sharedData, request, serverConfig, locationConfig));
	}

	static utils::shared_ptr<IBuilder<sharedData_t> > createPostResponseBuilder(
		const sharedData_t& sharedData, const request_t& request, const utils::shared_ptr<ServerConfig>& serverConfig,
		const utils::shared_ptr<LocationConfig>& locationConfig) {
		return utils::shared_ptr<IBuilder<sharedData_t> >(
			new PostResponseBuilder(sharedData, request, serverConfig, locationConfig));
	}

	static utils::shared_ptr<IBuilder<sharedData_t> > createDeleteResponseBuilder(
		const sharedData_t& sharedData, const request_t& request, const utils::shared_ptr<ServerConfig>& serverConfig,
		const utils::shared_ptr<LocationConfig>& locationConfig) {
		return utils::shared_ptr<IBuilder<sharedData_t> >(
			new DeleteResponseBuilder(sharedData, request, serverConfig, locationConfig));
	}

	static utils::shared_ptr<IBuilder<sharedData_t> > createPutResponseBuilder(
		const sharedData_t& sharedData, const request_t& request, const utils::shared_ptr<ServerConfig>& serverConfig,
		const utils::shared_ptr<LocationConfig>& locationConfig) {
		return utils::shared_ptr<IBuilder<sharedData_t> >(
			new PutResponseBuilder(sharedData, request, serverConfig, locationConfig));
	}

	static utils::shared_ptr<IBuilder<sharedData_t> > createHeadResponseBuilder(
		const sharedData_t& sharedData, const request_t& request, const utils::shared_ptr<ServerConfig>& serverConfig,
		const utils::shared_ptr<LocationConfig>& locationConfig) {
		return utils::shared_ptr<IBuilder<sharedData_t> >(
			new HeadResponseBuilder(sharedData, request, serverConfig, locationConfig));
	}

	static utils::shared_ptr<IBuilder<sharedData_t> > createErrorResponseBuilder(
		const int status, const sharedData_t& sharedData, const request_t& request,
		const utils::shared_ptr<ServerConfig>& serverConfig, const utils::shared_ptr<LocationConfig>& locationConfig) {
		return utils::shared_ptr<IBuilder<sharedData_t> >(
			new ErrorResponseBuilder(status, sharedData, serverConfig, locationConfig));
	}

	static utils::shared_ptr<IBuilder<sharedData_t> > createRedirectResponseBuilder(
		const unsigned int statusCode, const std::string& path, reactor::sharedData_t sharedData, request_t request,
		const utils::shared_ptr<ServerConfig>& serverConfig) {
		return utils::shared_ptr<IBuilder<sharedData_t> >(
			new RedirectResponseBuilder(statusCode, path, sharedData, request, serverConfig));
	}

	static utils::shared_ptr<IBuilder<sharedData_t> > createCgiResponseBuilder(
		const sharedData_t& sharedData, const request_t& request, const utils::shared_ptr<ServerConfig>& serverConfig,
		const utils::shared_ptr<LocationConfig>& locationConfig) {
		return utils::shared_ptr<IBuilder<sharedData_t> >(
			new CgiResponseBuilder(sharedData, request, serverConfig, locationConfig));
	}

	utils::shared_ptr<IBuilder<sharedData_t> > ClientResponseHandler::chooseBuilder() {
		try {
			if (_locationConfig.get() == u::nullptr_t)
				throw createErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_request, this->_serverConfig,
												 this->_locationConfig);
			if (this->_request->first == HTTP_ERROR)
				throw createErrorResponseBuilder(this->_request->second.getErrorCode(), this->_sharedData,
												 this->_request, this->_serverConfig, this->_locationConfig);

			const std::vector<enum HttpMethods> methods = this->_locationConfig->getDirectives(LIMIT_EXCEPT).asMedVec();

			if (std::find(methods.begin(), methods.end(), this->_request->second.getMethod()) == methods.end())
				throw createErrorResponseBuilder(METHOD_NOT_ALLOWED, this->_sharedData, this->_request,
												 this->_serverConfig, this->_locationConfig);
			if (this->_locationConfig->isRedirect()) {
				std::vector<std::string> rv = this->_locationConfig->getDirectives(RETURN).asStrVec();

				throw createRedirectResponseBuilder(utils::stoui(rv[0]), rv[1], this->_sharedData, this->_request,
													this->_serverConfig);
			}
			if (this->_locationConfig->isCgi())
				return createCgiResponseBuilder(this->_sharedData, this->_request, this->_serverConfig,
												this->_locationConfig);

			typedef utils::shared_ptr<IBuilder<sharedData_t> > (*ResponseBuilderFunction)(
				const sharedData_t&, const request_t&, const utils::shared_ptr<ServerConfig>&,
				const utils::shared_ptr<LocationConfig>&);
			ResponseBuilderFunction responseBuilderFunctions[] = {
				&createGetResponseBuilder, &createHeadResponseBuilder, &createPostResponseBuilder,
				&createDeleteResponseBuilder, &createPutResponseBuilder};

			enum HttpMethods method = this->_request->second.getMethod();
			if (method >= 0 && method < sizeof(responseBuilderFunctions) / sizeof(responseBuilderFunctions[0])) {
				return (*responseBuilderFunctions[method])(this->_sharedData, this->_request, this->_serverConfig,
														   this->_locationConfig);
			} else {
				return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
					BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
			}
		} catch (utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			return e;
		} catch (...) {
			return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
				INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
		}
		return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
}  // namespace reactor
