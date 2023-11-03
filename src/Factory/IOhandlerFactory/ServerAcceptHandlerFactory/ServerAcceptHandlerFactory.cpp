#include "ServerAcceptHandlerFactory.hpp"

ServerAcceptHandlerFactory::ServerAcceptHandlerFactory() {}
ServerAcceptHandlerFactory::~ServerAcceptHandlerFactory() {}

virtual utils::shared_ptr<AeventHandler> ServerAcceptHandlerFactory::createHandler(
	utils::shared_ptr<ServerData> sharedData) {
	return utils::static_pointer_cast<AeventHandler, ServerAcceptHandler>(
		utils::shared_ptr<ServerAcceptHandler>(new ServerAcceptHandler(sharedData)));
}