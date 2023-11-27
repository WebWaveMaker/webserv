#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t& sharedData)
		: AExeHandler(sharedData),
		  _request(sharedData->getRequest()),
		  _serverConfig(ServerManager::getInstance()->getServerConfig(this->getHandle())),
		  _locationConfig(_serverConfig->getLocationConfig(this->findLocationBlock())),
		  _keepalive(true),
		  _director(this->chooseBuilder()),
		  _registered(false) {}

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
			std::cerr << "ClientResponseHandler handleEvent removeHandlerIfNecessary" << std::endl;
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

	utils::shared_ptr<IBuilder<sharedData_t> > ClientResponseHandler::chooseBuilder() {
		try {
			if (this->_request->first == HTTP_ERROR)
				throw utils::shared_ptr<IBuilder<sharedData_t> >(
					new ErrorResponseBuilder(this->_request->second.getErrorCode(), this->_sharedData,
											 this->_serverConfig, this->_locationConfig));

			// if ((this->_request->second.getHeaders())["Connection"].compare("Keep-Alive") != 0)
			// 	this->_keepalive = false;
			std::vector<enum HttpMethods> methods = this->_locationConfig->getDirectives(LIMIT_EXCEPT).asMedVec();

			if (std::find(methods.begin(), methods.end(), this->_request->second.getMethod()) == methods.end())
				throw utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
					METHOD_NOT_ALLOWED, this->_sharedData, this->_serverConfig, this->_locationConfig));
			if (this->_locationConfig->isCgi()) {
				std::cerr << "cgi incgi incgi incgi incgi incgi incgi incgi incgi incgi incgi incgi incgi incgi incgi "
							 "incgi in"
						  << std::endl;
				return utils::shared_ptr<IBuilder<sharedData_t> >(new CgiResponseBuilder(
					this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
			}
			std::cerr << "nocginocginocginocginocginocginocginocginocginocginocgi" << std::endl;
			switch (this->_request->second.getMethod()) {
				case GET:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new GetResponseBuilder(
						this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case POST:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new PostResponseBuilder(
						this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case DELETE:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new DeleteResponseBuilder(
						this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case PUT:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new PutResponseBuilder(
						this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case HEAD:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new HeadResponseBuilder(
						this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case UNKNOWN:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
						BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
				default:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
						BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
					break;
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
