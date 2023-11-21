#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t& sharedData, va_list args)
		: AExeHandler(sharedData),
		  _request(*va_arg(args, request_t*)),
		  _serverConfig(ServerManager::getInstance()->getServerConfig(this->getHandle())),
		  _locationConfig(_serverConfig->getLocationConfig(_request->second.getRequestTarget())),
		  _director(this->chooseBuilder()),
		  _registered(false) {
		va_end(args);
	}

	ClientResponseHandler::~ClientResponseHandler() {}

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
			if (this->getBuffer().empty() && this->_director.getBuilderReadState() == RESOLVE) {
				Dispatcher::getInstance()->removeIOHandler(this->getHandle(), this->getType());
				Dispatcher::getInstance()->removeExeHandler(this);
				return;
			}

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
			std::vector<enum HttpMethods> methods = this->_locationConfig->getDirectives(LIMIT_EXCEPT).asMedVec();

			if (std::find(methods.begin(), methods.end(), this->_request->second.getMethod()) == methods.end())
				throw utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
					METHOD_NOT_ALLOWED, this->_sharedData, this->_serverConfig, this->_locationConfig));
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
