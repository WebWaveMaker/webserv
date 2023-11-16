#include "CgiResponseBuilder.hpp"

CgiResponseBuilder::CgiResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _cgiReadSharedData() {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(404, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->prepare();
}

reactor::sharedData_t CgiResponseBuilder::getProduct() {
	return this->_cgiReadSharedData;
}

void CgiResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(200));
}

bool CgiResponseBuilder::build() {
	// handleEvent에서 여기가 호출된다.
	// cgi write가 resolve가 먼저 -> read resolve 가능 -> CGI read와 write는 여기서 삭제 해야함

	return this->setBody();
}

std::string CgiResponseBuilder::makeCgiFullPath() {
	const std::string locRootPath = "." + this->_locationConfig.get()->getDirectives(ROOT).asString();
	const std::string serverRootPath = "." + this->_serverConfig.get()->getDirectives(ROOT).asString();
	const std::vector<std::string> cgiIndex = this->_locationConfig.get()->getDirectives(CGI_INDEX).asStrVec();
	const std::string uriPath = this->_request->second.getRequestTarget().erase(0, locRootPath.size());
	const std::string locationPath = this->_locationConfig.get()->getPath();

	// 먼저 .확장자가 존재하는지 찾는다. 있으면 /전까지 편집, 없으면 location만 제거하고 cgi실행
	size_t dotPos = uriPath.find('.');
	if (dotPos == std::string::npos) {
		for (std::vector<std::string>::const_iterator it = cgiIndex.begin(); it != cgiIndex.end(); ++it) {
			std::string cgiIndexTemp = *it;
			if ((*it).front() == '/')
				cgiIndexTemp = (*it).substr(1);
			std::string cgiFullPath = locRootPath + cgiIndexTemp;
			if (access(cgiFullPath.c_str(), X_OK) == 0)
				return cgiFullPath;
		}

		for (std::vector<std::string>::const_iterator it = cgiIndex.begin(); it != cgiIndex.end(); ++it) {
			std::string cgiIndexTemp = *it;
			if ((*it).front() == '/')
				cgiIndexTemp = (*it).substr(1);
			std::string cgiFullPath = locRootPath + cgiIndexTemp;
			if (access(cgiFullPath.c_str(), X_OK) == 0)
				return cgiFullPath;
		}
	} else {
		size_t slashPos = uriPath.find('/', dotPos);
		if (slashPos != std::string::npos)
			std::string uriFullPath = uriPath.substr(locationPath.size(), slashPos - 1);
		else
			std::string uriFullPath = uriPath.substr(locationPath.size());
		for (std::vector<std::string>::const_iterator it = cgiIndex.begin(); it != cgiIndex.end(); ++it) {
			std::string cgiIndexTemp = *it;
			if ((*it).front() == '/')
				cgiIndexTemp = (*it).substr(1);
			if (uriPath.compare(cgiIndexTemp) == 0) {
				if (access((locRootPath + cgiIndexTemp).c_str(), X_OK) == 0)
					return locRootPath + cgiIndexTemp;
				if (access((serverRootPath + cgiIndexTemp).c_str(), X_OK) == 0)
					return serverRootPath + cgiIndexTemp;
			}
		}
	}
	return std::string("");
}

bool CgiResponseBuilder::makeSocketPair() {
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, &this->_sv[0]) < 0) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		return false;
	}
	if (fcntl(this->_sv[0], F_SETFL, O_NONBLOCK) < 0 || fcntl(this->_sv[1], F_SETFL, O_NONBLOCK) < 0) {
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
		return false;
	}
	return true;
}

void CgiResponseBuilder::makeWriteSharedData() {
	this->_cgiWriteSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(this->_sv[0], EVENT_WRITE, std::vector<char>()));
	std::string body = this->_request.get()->second.getBody();
	for (std::string::size_type i = 0; i < body.size(); ++i)
		this->_cgiWriteSharedData.get()->getBuffer().push_back(body[i]);
}

void CgiResponseBuilder::prepare() {
	this->_cgiFullPath = this->makeCgiFullPath();
	if (this->_cgiFullPath.empty())
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(404, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->makeSocketPair() == false)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(500, this->_sharedData, this->_serverConfig, this->_locationConfig));
}