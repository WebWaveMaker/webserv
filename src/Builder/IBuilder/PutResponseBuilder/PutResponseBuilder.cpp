#include "PutResponseBuilder.h"

PutResponseBuilder::PutResponseBuilder(reactor::sharedData_t sharedData, request_t request,
									   const utils::shared_ptr<ServerConfig>& serverConfig,
									   const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData),
	  _request(request),
	  _serverConfig(serverConfig),
	  _locationConfig(locationConfig),
	  _readSharedData(new reactor::SharedData(FD_ERROR, EVENT_READ, std::vector<char>())),
	  _response(),
	  _path() {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(NOT_FOUND, this->_sharedData, this->_serverConfig, this->_locationConfig));
	if (_request.get()->second.getHeaders().find("Content-Length")->second == "0") {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	this->prepare();
}

PutResponseBuilder::~PutResponseBuilder() {}

bool PutResponseBuilder::updateFile(const std::string& path, const std::vector<char>& data) {
	if (data.empty()) {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}

	std::ofstream ofs(path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	if (ofs.is_open() == false) {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(FORBIDDEN, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	ofs.write(&data[0], data.size());
	ofs.close();
	this->_path = path;
	return true;
}

void PutResponseBuilder::createOrUpdateFile() {
	const std::string locPath =
		this->_locationConfig->getDirectives(ROOT).asString() + "/" + this->_request->second.getTargetFile();
	const std::string serverPath =
		this->_serverConfig->getDirectives(ROOT).asString() + this->_request->second.getTargetFile();

	const std::string& strData = this->_request->second.getBody();
	const std::vector<char> data(strData.begin(), strData.end());

	if (checkFileMode(locPath) == MODE_DIRECTORY) {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			UNSUPPORTED_MEDIA_TYPE, this->_sharedData, this->_serverConfig, this->_locationConfig));
	} else if (checkFileMode(serverPath) == MODE_DIRECTORY) {
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(new ErrorResponseBuilder(
			UNSUPPORTED_MEDIA_TYPE, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
	if (updateFile(locPath, data))
		return;
	if (updateFile(serverPath, data))
		return;
	throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
		new ErrorResponseBuilder(INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
}

reactor::sharedData_t PutResponseBuilder::getProduct() {
	return this->_sharedData;
}

void PutResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(OK));
}

void PutResponseBuilder::setHeader() {
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig, this->_path);

	headers.erase("Content-Type");
	headers["Content-Length"] = "0";
	this->_response.setHeaders(headers);
}

bool PutResponseBuilder::setBody() {
	if (this->_sharedData.get() == u::nullptr_t)
		return false;
	return true;
}

bool PutResponseBuilder::build() {
	return this->setBody();
}

void PutResponseBuilder::reset() {
	this->_response.reset();
}

void PutResponseBuilder::prepare() {
	this->createOrUpdateFile();
	this->setStartLine();
	this->setHeader();
	const std::string raw = this->_response.getRawStr() + CRLF;
	std::cerr << raw << std::endl;
	this->_sharedData->getBuffer().insert(this->_sharedData->getBuffer().begin(), raw.begin(), raw.end());
	this->_readSharedData->setState(RESOLVE);
}