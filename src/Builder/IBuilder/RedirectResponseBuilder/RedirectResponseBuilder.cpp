#include "RedirectResponseBuilder.hpp"

RedirectResponseBuilder::RedirectResponseBuilder(const int statusCode, reactor::sharedData_t sharedData,
												 const utils::shared_ptr<ServerConfig>& serverConfig,
												 const utils::shared_ptr<LocationConfig>& locationConfig) {}
RedirectResponseBuilder::~RedirectResponseBuilder() {}

virtual enum AsyncState RedirectResponseBuilder::getReadState() const {}
virtual void RedirectResponseBuilder::setReadState(enum AsyncState state) {}
virtual T RedirectResponseBuilder::getProduct() {}
virtual void RedirectResponseBuilder::setStartLine() {}
virtual void RedirectResponseBuilder::setHeader() {}
virtual bool RedirectResponseBuilder::setBody() {}
virtual void RedirectResponseBuilder::reset() {}
virtual bool RedirectResponseBuilder::build() {}
virtual void RedirectResponseBuilder::prepare() {}