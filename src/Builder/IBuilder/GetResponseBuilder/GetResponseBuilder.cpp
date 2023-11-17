#include "GetResponseBuilder.hpp"

GetResponseBuilder::GetResponseBuilder(reactor::sharedData_t sharedData, const request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _removed(false),
	  _path(),
	  _fd(FD_ERROR),
	  _readSharedData(),
	  _response() {
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

GetResponseBuilder::~GetResponseBuilder() {
	close(this->_fd);
}

reactor::sharedData_t GetResponseBuilder::getProduct() {
	return this->_readSharedData;
}

void GetResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(OK));
}

void GetResponseBuilder::setHeader() {
	struct stat fileInfo;

	if (stat(this->_path.c_str(), &fileInfo) == SYSTEMCALL_ERROR) {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	if (fileInfo.st_size == 0) {
		close(this->_fd);
		this->_fd = FD_ZERO_;
	}
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);
	headers["Content-Length"] = utils::lltos(fileInfo.st_size);
	this->_response.setHeaders(headers);
}

bool GetResponseBuilder::setBody() {
	if (this->_readSharedData.get() == u::nullptr_t)
		return false;
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().end(),
										  this->_readSharedData->getBuffer().begin(),
										  this->_readSharedData->getBuffer().end());
	this->_readSharedData->getBuffer().clear();
	if (this->_readSharedData->getState() == RESOLVE && this->_removed == false) {
		reactor::Dispatcher::getInstance()->removeIOHandler(this->_readSharedData->getFd(),
															this->_readSharedData->getType());
		this->_removed = true;
		return true;
	}
	return true;
}

void GetResponseBuilder::reset() {
	this->_response.reset();
	this->_sharedData->getBuffer().clear();
	this->_readSharedData->getBuffer().clear();
}
bool GetResponseBuilder::build() {
	return this->setBody();
}

fd_t GetResponseBuilder::findReadFile() {
	const std::string locPath = "." + this->_locationConfig->getDirectives(ROOT).asString();
	const std::string serverPath = "." + this->_serverConfig->getDirectives(ROOT).asString();
	const std::string targetFile = this->_request->second.getTargetFile();

	this->_path = locPath + targetFile;
	if (access(this->_path.c_str(), R_OK) == 0)
		return utils::makeFd(this->_path.c_str(), "r");
	this->_path = serverPath + targetFile;
	if (access(this->_path.c_str(), R_OK) == 0)
		return utils::makeFd(this->_path.c_str(), "r");
	return FD_ERROR;
}

fd_t GetResponseBuilder::fileProcessing() {
	if (this->checkFileMode("." + this->_locationConfig->getDirectives(ROOT).asString() +
							this->_request->second.getTargetFile()) == MODE_DIRECTORY)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new RedirectResponseBuilder(MOVED_PERMANENTLY, this->_request->second.getRequestTarget() + "/",
										this->_sharedData, this->_request, this->_serverConfig));
	return this->findReadFile();
}

std::vector<std::string> GetResponseBuilder::readDir(const std::string& path) {
	DIR* dirp;
	struct dirent* dp;

	if ((dirp = opendir(path.c_str())) == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	dp = readdir(dirp);	 // skip current directory
	dp = readdir(dirp);	 // skip parent directory
	if (dp == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
	std::vector<std::string> dirVec;
	while ((dp = readdir(dirp)) != u::nullptr_t) {
		if (dp == u::nullptr_t)
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
				INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
		dirVec.push_back(dp->d_name);
	}
	closedir(dirp);
	return dirVec;
};

void GetResponseBuilder::makeListHtml(const std::string& path, const std::vector<std::string>& dirVec) {
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
	headers["Content-Length"] = utils::lltos(html.size());
	headers["Content-Type"] = "text/html";
	this->_response.setHeaders(headers);
	const std::string raw = this->_response.getRawStr();
	this->_readSharedData = utils::shared_ptr<reactor::SharedData>(
		new reactor::SharedData(FD_LISTING, EVENT_READ, std::vector<char>(html.begin(), html.end())));
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->setReadState(RESOLVE);
	this->_removed = true;
}

fd_t GetResponseBuilder::directoryListing() {
	std::cout << "directory listing" << std::endl;
	if (this->_locationConfig->getDirectives(AUTOINDEX).asBool() == false)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(FORBIDDEN, this->_sharedData, this->_serverConfig, this->_locationConfig));
	const std::string path = "." + this->_locationConfig->getOwnRoot();
	const std::vector<std::string> dirVec = this->readDir(path);
	makeListHtml(path, dirVec);

	return FD_LISTING;
}

fd_t GetResponseBuilder::directoryProcessing() {
	if (this->checkFileMode("." + this->_locationConfig->getDirectives(ROOT).asString() +
							this->_request->second.getTargetFile()) == MODE_FILE)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->_locationConfig->getOwnIndex().empty())
		return this->directoryListing();
	const std::string locPath = "." + this->_locationConfig->getDirectives(ROOT).asString();
	const std::string serverPath = "." + this->_serverConfig->getDirectives(ROOT).asString();
	const std::vector<std::string> indexVec = this->_locationConfig->getDirectives(INDEX).asStrVec();

	for (std::vector<std::string>::const_iterator cit = indexVec.begin(); cit != indexVec.end(); ++cit) {
		this->_path = locPath + *cit;
		if (access(this->_path.c_str(), R_OK) == 0)
			return utils::makeFd(this->_path.c_str(), "r");
	}
	for (std::vector<std::string>::const_iterator cit = indexVec.begin(); cit != indexVec.end(); ++cit) {
		this->_path = serverPath + *cit;
		if (access(this->_path.c_str(), R_OK) == 0)
			return utils::makeFd(this->_path.c_str(), "r");
	}
	return FD_ERROR;
}

void GetResponseBuilder::prepare() {
	this->_fd = this->checkOurFileMode(this->_request->second.getRequestTarget()) == MODE_FILE
					? this->fileProcessing()
					: this->directoryProcessing();
	if (this->_fd == FD_ERROR)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (this->_fd == FD_LISTING)
		return;
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr();
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(this->_fd, EVENT_READ, std::vector<char>()));
	if (this->_fd == FD_ZERO_) {
		this->setReadState(RESOLVE);
		this->_removed = true;
		this->getProduct()->getBuffer().push_back('\r');
		this->getProduct()->getBuffer().push_back('\n');
		return;
	}
	reactor::Dispatcher::getInstance()->registerIOHandler<reactor::FileReadHandlerFactory>(this->_readSharedData);
}