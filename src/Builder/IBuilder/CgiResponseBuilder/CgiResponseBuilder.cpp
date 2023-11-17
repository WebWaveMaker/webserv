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
	this->inItInterpreterMap();
	this->prepare();
}

bool CgiResponseBuilder::setBody() {
	if (this->_cgiReadSharedData.get() == u::nullptr_t)
		return false;

	// cgi로 부터 받은 status: 200 ok 부분을 startline형식으로 변경해야함
	this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().end(),
												this->_cgiReadSharedData.get()->getBuffer().begin(),
												this->_cgiReadSharedData.get()->getBuffer().end());
	this->_cgiReadSharedData.get()->getBuffer().clear();
	if (this->_cgiReadSharedData.get()->getState() == TERMINATE && this->_removed == false) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_cgiReadSharedData.get()->getFd(),
															this->_cgiReadSharedData.get()->getType());
		return true;
	}
	return true;
}

void CgiResponseBuilder::replaceStartLine() {}

void CgiResponseBuilder::inItInterpreterMap() {
	this->_interpreterMap["pl"] = "perl";
	this->_interpreterMap["py"] = "python";
	this->_interpreterMap["rb"] = "ruby";
	this->_interpreterMap["php"] = "php";
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
	const std::string locRootPath = this->_locationConfig.get()->getDirectives(ROOT).asString();
	const std::string serverRootPath = this->_serverConfig.get()->getDirectives(ROOT).asString();
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
			std::string uriFullPath = uriPath.substr(locationPath.size(), slashPos - 1);  //뒤에 '/' 없음
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

std::string CgiResponseBuilder::makeQueryString() {
	std::string queryString = this->_request->second.getRequestTarget();

	if (queryString.empty())
		return std::string("");

	size_t questPos = queryString.find('?');

	if (questPos != std::string::npos) {
		queryString.erase(0, questPos + 1);
		return (queryString);
	} else
		return std::string("");
}

std::string CgiResponseBuilder::makePathInfo() {
	std::string pathInfo = this->_request->second.getRequestTarget();
	std::string locRootPath = this->_locationConfig.get()->getDirectives(ROOT).asString();
	pathInfo.erase(0, locRootPath.size());

	size_t dotPos = pathInfo.find('.');
	if (dotPos != std::string::npos) {
		size_t slashPos = pathInfo.find('/', dotPos);
		pathInfo.erase(0, slashPos + 1);
		size_t questPos = pathInfo.find('?');
		if (questPos == std::string::npos)
			return (pathInfo);
		pathInfo.erase(questPos);
		return (pathInfo);
	} else {
		size_t questPos = pathInfo.find('?');
		if (questPos != std::string::npos)
			pathInfo.erase(questPos);
	}
	return (pathInfo);
}

bool CgiResponseBuilder::doFork() {
	pid_t pid = fork();
	if (pid == -1)
		return false;
	if (pid == 0) {
		close(this->_sv[0]);
		char** envp = this->setEnvp();
		char** args = this->makeArgs();
		dup2(this->_sv[1], STDIN_FILENO);
		dup2(this->_sv[1], STDOUT_FILENO);
		close(this->_sv[1]);
		execve(args[0], args, envp);
	}
	return true;
}

char** CgiResponseBuilder::makeArgs() {
	std::vector<std::string> argsVec;
	std::string interpreter = this->makeInterpreter();
	if (!interpreter.empty())
		argsVec.push_back(interpreter);
	argsVec.push_back(this->_cgiFullPath);
	char** args = new char*[argsVec.size() + 1];
	for (int i = 0; i < argsVec.size(); ++i)
		args[i] = argsVec[i].c_str();
	args[argsVec.size() + 1] = NULL;
	return args;
}

void CgiResponseBuilder::addCgiEnvp(std::vector<std::string>& cgiEnvpVec, const std::string& key,
									const std::string& value) {
	cgiEnvpVec.push_back(key + "=" + value);
}

char** CgiResponseBuilder::setEnvp() {
	char** envp = ServerManager::getInstance()->getEnvp();

	std::vector<std::string> cgiEnvpVec;

	std::string method = this->_request.get()->second.getMethodStr();
	std::string scriptName = this->_cgiFullPath;
	std::string queryString = this->makeQueryString();

	std::string serverName = this->_request.get()->second.getHeaders()["Host"];
	size_t colPos = serverName.find(':');
	if (colPos != std::string::npos)
		serverName.erase(colPos);

	int serverPortInt = this->_serverConfig.get()->getDirectives(LISTEN).asUint();
	std::ostringstream oss;
	oss << serverPortInt;
	std::string serverPort = oss.str();
	oss.str("");
	oss.clear();

	std::string serverProtocol = "HTTP/1.1";
	std::string gatewayInterface = "CGI/1.1";
	std::string contentType = (this->_request.get()->second.getHeaders())["Content-Type"];
	std::string contentLength = (this->_request.get()->second.getHeaders())["Content-Length"];
	std::string remoteAddr = ServerManager::getInstance()->getClientIp(this->_sharedData.get()->getFd());

	this->addCgiEnvp(cgiEnvpVec, "REQUEST_METHOD", method);
	this->addCgiEnvp(cgiEnvpVec, "SCRIPT_NAME", scriptName);
	this->addCgiEnvp(cgiEnvpVec, "QUERY_STRING", queryString);
	this->addCgiEnvp(cgiEnvpVec, "SERVER_NAME", serverName);
	this->addCgiEnvp(cgiEnvpVec, "SERVER_PORT", serverPort);
	this->addCgiEnvp(cgiEnvpVec, "SERVER_PROTOCOL", serverProtocol);
	this->addCgiEnvp(cgiEnvpVec, "GATEWAY_INTERFACE", gatewayInterface);
	this->addCgiEnvp(cgiEnvpVec, "CONTENT_TYPE", contentType);
	this->addCgiEnvp(cgiEnvpVec, "CONTENT_LENGTH", contentLength);
	this->addCgiEnvp(cgiEnvpVec, "REMOTE_ADDR", remoteAddr);

	int envpLen;
	for (envpLen = 0; envp[envpLen] != NULL; ++envpLen)
		;
	char** newEnvp = new char*[cgiEnvpVec.size() + envpLen + 1];
	int idx;
	for (int idx = 0; idx < envpLen; ++idx)
		newEnvp[idx] = envp[idx];
	for (size_t i = 0; i < cgiEnvpVec.size(); ++i) {
		newEnvp[idx] = cgiEnvpVec[i].c_str();
		idx++;
	}
	newEnvp[idx] = NULL;
	return (newEnvp);
}

//pl = perl, py = python, php = php, rb = ruby 절대경로 필요
std::string CgiResponseBuilder::makeInterpreter() {
	std::string extension = this->makeExtension();
	if (extension.empty())
		return std::string("");
	std::vector<std::string> pathEnvp = this->parsPathEnvp();
	std::string interpreter;
	std::map<std::string, std::string>::iterator it = interpreterMap.find(extension);
	if (it == interpreterMap.end())
		return std::string("");
	interpreter = it->second;
	for (size_t i = 0; i < pathEnvp.size(); ++i) {
		std::string exePath = pathEnvp[i] + "/" + interpreter;
		if (access(exePath.c_str(), F_OK | X_OK) == 0)
			return (exePath);
	}
	return std::string("");
}

std::vector<std::string> CgiResponseBuilder::parsPathEnvp() {
	char** envp = ServerManager::getInstance()->getEnvp();
	std::vector<std::string> result;
	int i;

	i = 0;
	while (envp && envp[i]) {
		if (std::memcpy(envp[i], "PATH=", 5) == 0)
			result = utils::split(std::string(envp[i]), ":");
		i++;
	}
	return (result);
}

std::string CgiResponseBuilder::makeExtension() {
	size_t dotPos = this->_cgiFullPath.find('.');
	if (dotPos == std::string::npos)
		return std::string("");
	size_t slashPos = this->_cgiFullPath.find('/', dotPos);
	std::string extension;
	if (slashPos != std::string::npos)
		extension = this->_cgiFullPath.substr(dotPos + 1, slashPos - dotPos - 1);
	else
		extension = this->_cgiFullPath.substr(dotPos + 1);
	return extension;
}

void CgiResponseBuilder::prepare() {
	this->_cgiFullPath = this->makeCgiFullPath();
	if (this->_cgiFullPath.empty())
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(404, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->makeSocketPair() == false)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(500, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->makeWriteSharedData();
	this->doFork();
	// CGI 프로세스에개 write
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::ClientWriteHandlerFactory>(
		this->_cgiWriteSharedData);
	// CGI로 부터 read
	this->_cgiReadSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(this->_sv[0], EVENT_READ, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::ClientReadHandlerFactory>(this->_cgiReadSharedData);
}
