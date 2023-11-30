#include "HeadResponseBuilder.hpp"

HeadResponseBuilder::HeadResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
										 const utils::shared_ptr<ServerConfig>& serverConfig,
										 const utils::shared_ptr<LocationConfig>& locationConfig,
										 SessionData* sessionData)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _path(),
	  _readSharedData(),
	  _response(),
	  _sessionData(sessionData) {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->_locationConfig->isRedirect()) {
		std::vector<std::string> rv = this->_locationConfig->getDirectives(RETURN).asStrVec();

		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new RedirectResponseBuilder(
			utils::stoui(rv[0]), rv[1], this->_sharedData, this->_request, this->_serverConfig));
	}
	this->prepare();
}

HeadResponseBuilder::~HeadResponseBuilder() {}

reactor::sharedData_t HeadResponseBuilder::getProduct() {
	return this->_readSharedData;
}

void HeadResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(OK));
}

void HeadResponseBuilder::setHeader() {
	struct stat fileInfo;

	if (stat(this->_path.c_str(), &fileInfo) == SYSTEMCALL_ERROR) {
		throw ErrorResponseBuilder::createErrorResponseBuilder(INTERNAL_SERVER_ERROR, this->_sharedData,
															   this->_serverConfig, this->_locationConfig);
	}
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);
	headers[CONTENT_LENGTH] = utils::lltos(fileInfo.st_size);
	this->_response.setHeaders(headers);
}

bool HeadResponseBuilder::setBody() {
	if (this->_sharedData.get() == u::nullptr_t)
		return false;
	return true;
}

void HeadResponseBuilder::reset() {
	this->_response.reset();
	this->_sharedData->getBuffer().clear();
	this->_readSharedData->getBuffer().clear();
}

bool HeadResponseBuilder::build() {
	return this->setBody();
}

std::string HeadResponseBuilder::findReadFile() {
	const std::string locPath = this->_locationConfig->getDirectives(ROOT).asString();
	const std::string serverPath = this->_serverConfig->getDirectives(ROOT).asString();
	const std::string targetFile = this->_request->second.getTargetFile();
	std::string path;

	path = locPath + targetFile;
	if (access(path.c_str(), R_OK) == 0)
		return path;
	path = serverPath + targetFile;
	if (access(path.c_str(), R_OK) == 0)
		return path;
	return "";
}

std::string HeadResponseBuilder::fileProcessing() {
	const enum FileMode mode = this->checkFileMode(this->_locationConfig->getDirectives(ROOT).asString() +
												   this->_request->second.getTargetFile());
	const enum FileMode serverMode = this->checkFileMode(this->_serverConfig->getDirectives(ROOT).asString() +
														 this->_request->second.getTargetFile());
	if (mode == MODE_DIRECTORY)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new RedirectResponseBuilder(MOVED_PERMANENTLY, this->_request->second.getRequestTarget() + "/",
										this->_sharedData, this->_request, this->_serverConfig));
	if (mode == MODE_ERROR && serverMode == MODE_ERROR)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	return this->findReadFile();
}

std::vector<std::string> HeadResponseBuilder::readDir(const std::string& path) {
	if (path == "")
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(FORBIDDEN, this->_sharedData, this->_serverConfig, this->_locationConfig));
	DIR* dirp;
	struct dirent* dp;

	if ((dirp = opendir(path.c_str())) == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	std::vector<std::string> dirVec;
	while ((dp = readdir(dirp)) != u::nullptr_t) {
		if (dp->d_name[0] == '.')
			continue;
		if (dp == u::nullptr_t)
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
				INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
		dirVec.push_back(dp->d_name);
	}
	closedir(dirp);
	return dirVec;
};

void HeadResponseBuilder::makeListHtml(const std::string& path, const std::vector<std::string>& dirVec) {
	const std::string requestTarget = this->_request->second.getRequestTarget();
	struct stat fileStat;

	std::string html = LIST_HTML_HEADER(requestTarget);

	for (std::vector<std::string>::const_iterator cit = dirVec.begin(); cit != dirVec.end(); ++cit) {
		if (stat((path + "/" + *cit).c_str(), &fileStat) == SYSTEMCALL_ERROR)
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
				INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
		if (S_ISDIR(fileStat.st_mode))
			html += "<a href=\"" + *cit + "\"/>" + *cit + "/</a>";
		else
			html += "<a href=\"" + *cit + "\">" + *cit + "</a>";
		html += "                                               " +
				utils::formatTime(fileStat.st_birthtimespec.tv_sec, logTimeFormat::dirListFormat) +
				"                   " + (S_ISDIR(fileStat.st_mode) ? "-" : utils::lltos(fileStat.st_size)) + "\r\n";
	}
	html += "</pre><hr></body>\r\n</html>";
	this->setStartLine();
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig);
	headers[CONTENT_LENGTH] = utils::lltos(html.size());
	headers["Content-Type"] = "text/html";
	this->_response.setHeaders(headers);
	const std::string raw = this->_response.getRawStr() + html;
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(FD_LISTING, EVENT_READ, std::vector<char>()));
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->setReadState(RESOLVE);
}

std::string HeadResponseBuilder::directoryListing() {
	// std::cout << "directory listing" << std::endl;
	if (this->_locationConfig->getDirectives(AUTOINDEX).asBool() == false)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(FORBIDDEN, this->_sharedData, this->_serverConfig, this->_locationConfig));
	const std::string path = this->_locationConfig->getPath() == this->_request->second.getRequestTarget()
								 ? this->_locationConfig->getOwnRoot()
								 : "";
	const std::vector<std::string> dirVec = this->readDir(path);
	makeListHtml(path, dirVec);

	return "listing";
}

std::string HeadResponseBuilder::directoryProcessing() {
	if (this->checkFileMode(this->_locationConfig->getDirectives(ROOT).asString() +
							this->_request->second.getTargetFile()) == MODE_FILE)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->_locationConfig->getOwnIndex().empty())
		return this->directoryListing();
	const std::string locPath = this->_locationConfig->getDirectives(ROOT).asString();
	const std::string serverPath = this->_serverConfig->getDirectives(ROOT).asString();
	const std::vector<std::string> indexVec = this->_locationConfig->getDirectives(INDEX).asStrVec();
	std::string path;

	for (std::vector<std::string>::const_iterator cit = indexVec.begin(); cit != indexVec.end(); ++cit) {
		path = locPath + *cit;
		if (access(path.c_str(), R_OK) == 0)
			return path;
	}
	for (std::vector<std::string>::const_iterator cit = indexVec.begin(); cit != indexVec.end(); ++cit) {
		path = serverPath + *cit;
		if (access(path.c_str(), R_OK) == 0)
			return path;
	}
	return "";
}

void HeadResponseBuilder::prepare() {
	this->_path = this->checkOurFileMode(this->_request->second.getRequestTarget()) == MODE_FILE
					  ? this->fileProcessing()
					  : this->directoryProcessing();
	if (this->_path == "")
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->_path == "listing")
		return;
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr();
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(FD_ERROR, EVENT_READ, std::vector<char>()));
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->setReadState(RESOLVE);
}

utils::shared_ptr<IBuilder<reactor::sharedData_t> > HeadResponseBuilder::createHeadResponseBuilder(
	const reactor::sharedData_t& sharedData, const request_t& request,
	const utils::shared_ptr<ServerConfig>& serverConfig, const utils::shared_ptr<LocationConfig>& locationConfig,
	SessionData* sessionData) {
	return utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
		new HeadResponseBuilder(sharedData, request, serverConfig, locationConfig, sessionData));
}
