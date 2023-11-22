#include "CgiResponseBuilder.hpp"

CgiResponseBuilder::CgiResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _cgiReadSharedData(),
	  _startLineState(false) {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(404, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->inItInterpreterMap();
	this->prepare();
}

void CgiResponseBuilder::inItInterpreterMap() {
	this->_interpreterMap["pl"] = "perl";
	this->_interpreterMap["py"] = "python";
	this->_interpreterMap["rb"] = "ruby";
	this->_interpreterMap["php"] = "php";
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
	if (this->doFork() == false) {
		close(this->_sv[0]);
		close(this->_sv[1]);
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(500, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	close(this->_sv[1]);
	// CGI 프로세스에개 write
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::ClientWriteHandlerFactory>(
		this->_cgiWriteSharedData);
	// CGI로 부터 read
	this->_cgiReadSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(this->_sv[0], EVENT_READ, std::vector<char>()));
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::ClientReadHandlerFactory>(this->_cgiReadSharedData);
}

// handleEvent에서 여기가 호출된다.
bool CgiResponseBuilder::build() {
	// cgi에 대한 WriteHandler를 삭제합니다.
	int status;
	if (this->_sharedData->getState() == TERMINATE) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_cgiWriteSharedData->getFd(),
															this->_cgiWriteSharedData->getType());
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_cgiReadSharedData->getFd(),
															this->_cgiReadSharedData->getType());
		if (waitpid(this->_cgiPid, &status, WNOHANG) < 0)
			kill(this->_cgiPid, SIGTERM);
		return false;
	}
	if (waitpid(this->_cgiPid, &status, WNOHANG) > 0) {
		if (WIFEXITED(status) == false) {
			reactor::Dispatcher::getInstance()->removeIOHandler(this->_cgiWriteSharedData->getFd(),
																this->_cgiWriteSharedData->getType());
			reactor::Dispatcher::getInstance()->removeIOHandler(this->_cgiReadSharedData->getFd(),
																this->_cgiReadSharedData->getType());
			throw false;
		}
	} else {
		if (std::difftime(std::time(NULL), this->_cgiTime) >= 180) {
			kill(this->_cgiPid, SIGTERM);
			throw false;
		}
	}
	if ((this->_request->first == DONE || this->_request->first == LONG_BODY_DONE) &&
		this->_cgiWriteSharedData->getBuffer().empty()) {
		this->_cgiWriteSharedData->setState(RESOLVE);
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_cgiWriteSharedData->getFd(),
															this->_cgiWriteSharedData->getType());
	}
	return this->setBody();
}

bool CgiResponseBuilder::setBody() {
	if (this->_cgiReadSharedData.get() == u::nullptr_t)
		return false;

	// cgi로 부터 받은 status: 200 ok 부분을 startline형식으로 변경해야함
	// cgi에 대한 read이벤트를 삭제합니다.
	if (this->_startLineState == false)
		this->replaceStartLine();
	else {
		this->_cgiWriteSharedData->getBuffer().insert(this->_cgiWriteSharedData->getBuffer().end(),
													  this->_request->second.getBody().begin(),
													  this->_request->second.getBody().end());
		this->_request->second.getBody().clear();
		this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().end(),
													this->_cgiReadSharedData.get()->getBuffer().begin(),
													this->_cgiReadSharedData.get()->getBuffer().end());
		this->_cgiReadSharedData.get()->getBuffer().clear();
	}
	if (this->_cgiReadSharedData.get()->getState() == TERMINATE && this->_cgiWriteSharedData->getState() == RESOLVE) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_cgiReadSharedData.get()->getFd(),
															this->_cgiReadSharedData.get()->getType());
		this->_cgiReadSharedData->setState(RESOLVE);
		return true;
	}
	return true;
}

std::string CgiResponseBuilder::makePathInfo() {
	std::string pathInfo = this->_request->second.getRequestTarget();
	std::string locPath = this->_locationConfig.get()->getPath();

	pathInfo.erase(0, locPath.size());

	size_t dotPos = pathInfo.find('.');
	if (dotPos != std::string::npos) {
		size_t slashPos = pathInfo.find('/', dotPos);
		if (slashPos != std::string::npos)
			pathInfo.erase(0, slashPos);
		size_t questPos = pathInfo.find('?');
		if (questPos == std::string::npos)
			return pathInfo;
		pathInfo.erase(questPos);
		return pathInfo;
	} else {
		if (pathInfo.empty())
			return pathInfo;
		pathInfo = "/" + pathInfo;
		size_t questPos = pathInfo.find('?');
		if (questPos == std::string::npos)
			return pathInfo;
		pathInfo.erase(questPos);
		return pathInfo;
	}
}

std::string CgiResponseBuilder::makeUriPath() {
	std::string uriCgiPath = this->_request->second.getRequestTarget();
	std::string locPath = this->_locationConfig.get()->getPath();
	uriCgiPath.erase(0, locPath.size());

	size_t dotPos = uriCgiPath.find('.');
	if (dotPos != std::string::npos) {
		size_t slashPos = uriCgiPath.find('/', dotPos);
		if (slashPos != std::string::npos)
			uriCgiPath.erase(slashPos);
		size_t questPos = uriCgiPath.find('?');
		if (questPos == std::string::npos)
			return (uriCgiPath);
		uriCgiPath.erase(questPos);
		return (uriCgiPath);
	} else {
		size_t questPos = uriCgiPath.find('?');
		if (questPos != std::string::npos)
			uriCgiPath.erase(questPos);
	}
	return (uriCgiPath);
}

std::string CgiResponseBuilder::makeCgiFullPath() {
	const std::string locRootPath = this->_locationConfig.get()->getDirectives(ROOT).asString();
	const std::string serverRootPath = this->_serverConfig.get()->getDirectives(ROOT).asString();
	const std::vector<std::string> cgiIndex = this->_locationConfig.get()->getDirectives(CGI_INDEX).asStrVec();
	const std::string uriPath = this->makeUriPath();
	const std::string locationPath = this->_locationConfig.get()->getPath();

	// 먼저 .확장자가 존재하는지 찾는다. 있으면 /전까지 편집, 없으면 location만 제거하고 cgi실행
	size_t dotPos = uriPath.find('.');
	if (dotPos == std::string::npos) {
		for (std::vector<std::string>::const_iterator it = cgiIndex.begin(); it != cgiIndex.end(); ++it) {
			std::string cgiIndexTemp = *it;
			if ((*it).front() == '/')
				cgiIndexTemp = (*it).substr(1);
			if (access((locRootPath + cgiIndexTemp).c_str(), X_OK) == 0)
				return locRootPath + cgiIndexTemp;
			if (access((serverRootPath + cgiIndexTemp).c_str(), X_OK) == 0)
				return serverRootPath + cgiIndexTemp;
		}
	} else {
		for (std::vector<std::string>::const_iterator it = cgiIndex.begin(); it != cgiIndex.end(); ++it) {
			std::string cgiIndexTemp = *it;
			if ((*it).front() == '/')
				cgiIndexTemp = (*it).substr(1);
			if ((locRootPath + cgiIndexTemp).compare(uriPath) == 0) {
				if (access((locRootPath + cgiIndexTemp).c_str(), X_OK) == 0)
					return locRootPath + cgiIndexTemp;
			} else if ((serverRootPath + cgiIndexTemp).compare(uriPath) == 0) {
				if (access((serverRootPath + cgiIndexTemp).c_str(), X_OK) == 0)
					return serverRootPath + cgiIndexTemp;
			}
		}
	}
	return std::string("");
}

void CgiResponseBuilder::cgiStartLineInsert() {
	const std::string delimiter = "\r\n";
	std::vector<char>& readBuffer = this->_cgiReadSharedData->getBuffer();
	std::vector<char>::iterator it =
		std::search(readBuffer.begin(), readBuffer.end(), delimiter.begin(), delimiter.end());

	if (it != readBuffer.end()) {
		readBuffer.erase(readBuffer.begin(), it + delimiter.size());
		std::string startLine = this->_startLine[0] + " " + this->_startLine[1] + " " + this->_startLine[2] + "\r\n";
		readBuffer.insert(readBuffer.begin(), startLine.begin(), startLine.end());
		this->_startLineState = true;
	}
}

void CgiResponseBuilder::replaceStartLine() {
	std::vector<char>& readBuffer = this->_cgiReadSharedData->getBuffer();
	if (readBuffer.size() == 0)
		return;
	std::string msg;

	const char crlf[] = {'\r', '\n'};
	std::vector<char>::iterator crlfIt = std::find_first_of(readBuffer.begin(), readBuffer.end(), crlf, crlf + 2);

	if (crlfIt == readBuffer.end())
		return;

	msg.assign(readBuffer.begin(), crlfIt);
	std::stringstream ss;
	ss << msg;
	ss >> this->_startLine[0] >> this->_startLine[1] >> this->_startLine[2];
	if (this->_startLine[0].empty() || this->_startLine[1].empty() || this->_startLine[2].empty()) {
		this->_startLine->clear();
		return;
	}
	if (this->_startLine[0].compare("Status:") == 0) {
		this->_startLine[0] = "HTTP/1.1";
		this->_startLineState = true;
	} else {
		this->_startLine[0] = "HTTP/1.1";
		this->_startLine[1] = "200";
		this->_startLine[2] = "OK";
		std::string startLine = this->_startLine[0] + " " + this->_startLine[1] + " " + this->_startLine[2] + "\r\n";
		readBuffer.insert(readBuffer.begin(), startLine.begin(), startLine.end());
		this->_startLineState = true;
		return;
	}
	this->cgiStartLineInsert();
}

reactor::sharedData_t CgiResponseBuilder::getProduct() {
	return this->_cgiReadSharedData;
}

void CgiResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(200));
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
	this->_cgiWriteSharedData->getBuffer().insert(this->_cgiWriteSharedData->getBuffer().end(),
												  this->_request->second.getBody().begin(),
												  this->_request->second.getBody().end());
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

bool CgiResponseBuilder::doFork() {
	this->_cgiPid = fork();
	if (this->_cgiPid == -1) {
		close(this->_sv[0]);
		close(this->_sv[1]);
		return false;
	}
	this->_cgiTime = std::time(NULL);
	if (this->_cgiPid == 0) {
		close(this->_sv[0]);
		char** envp = this->setEnvp();
		char** args = this->makeArgs();
		if (dup2(this->_sv[1], STDIN_FILENO) == -1) {
			close(this->_sv[1]);
			exit(1);
		}
		if (dup2(this->_sv[1], STDOUT_FILENO) == -1) {
			close(this->_sv[1]);
			exit(1);
		}
		close(this->_sv[1]);
		execve(args[0], args, envp);
		exit(1);
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
	for (size_t i = 0; i < argsVec.size(); ++i) {
		args[i] = strdup(const_cast<char*>(argsVec[i].c_str()));
	}
	args[argsVec.size()] = NULL;
	return args;
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

	std::string serverPort = utils::itos(this->_serverConfig.get()->getDirectives(LISTEN).asUint());

	std::string serverProtocol = "HTTP/1.1";
	std::string gatewayInterface = "CGI/1.1";
	std::string contentType = (this->_request.get()->second.getHeaders())["Content-Type"];
	std::string contentLength = (this->_request.get()->second.getHeaders())["Content-Length"];
	if (contentLength.empty())
		contentLength = "0";
	std::string remoteAddr = ServerManager::getInstance()->getClientIp(this->_sharedData.get()->getFd());
	std::string pathInfo = this->makePathInfo();

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
	this->addCgiEnvp(cgiEnvpVec, "PATH_INFO", pathInfo);

	int envpLen;
	for (envpLen = 0; envp[envpLen] != NULL; ++envpLen)
		;
	char** newEnvp = new char*[cgiEnvpVec.size() + envpLen + 1];
	int idx;
	for (idx = 0; idx < envpLen; ++idx)
		newEnvp[idx] = envp[idx];
	for (size_t i = 0; i < cgiEnvpVec.size(); ++i) {
		newEnvp[idx] = strdup(const_cast<char*>(cgiEnvpVec[i].c_str()));
		idx++;
	}
	newEnvp[idx] = NULL;
	return (newEnvp);
}

void CgiResponseBuilder::addCgiEnvp(std::vector<std::string>& cgiEnvpVec, const std::string& key,
									const std::string& value) {
	cgiEnvpVec.push_back(key + "=" + value);
}

//pl = perl, py = python, php = php, rb = ruby 절대경로 필요
std::string CgiResponseBuilder::makeInterpreter() {
	std::string extension = this->makeExtension();
	if (extension.empty())
		return std::string("");
	std::vector<std::string> pathEnvp = this->parsPathEnvp();
	std::string interpreter;
	std::map<std::string, std::string>::iterator it = this->_interpreterMap.find(extension);
	if (it == this->_interpreterMap.end())
		return std::string("");
	interpreter = it->second;
	for (size_t i = 0; i < pathEnvp.size(); ++i) {
		std::string exePath = pathEnvp[i] + "/" + interpreter;
		if (access(exePath.c_str(), F_OK | X_OK) == 0)
			return (exePath);
	}
	return std::string("");
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

CgiResponseBuilder::~CgiResponseBuilder() {
	close(this->_sv[0]);
}

void CgiResponseBuilder::setHeader() {}

void CgiResponseBuilder::reset() {}
