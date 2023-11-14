#include "DeleteResponseBuilder.hpp"

DeleteResponseBuilder::DeleteResponseBuilder(reactor::sharedData_t sharedData, request_t request,
											 const utils::shared_ptr<ServerConfig>& serverConfig,
											 const utils::shared_ptr<LocationConfig>& locationConfig)
	: _sharedData(sharedData), _request(request), _serverConfig(serverConfig), _locationConfig(locationConfig) {
	if (_locationConfig.get() == u::nullptr_t)
		throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
			new ErrorResponseBuilder(404, this->_sharedData, this->_serverConfig, this->_locationConfig));
	this->prepare();
}

DeleteResponseBuilder::~DeleteResponseBuilder() {}

std::vector<std::string> split(const std::string& str, char delim) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)) {
		if (!token.empty()) {
			tokens.push_back(token);
		}
	}
	return tokens;
}

std::string mergePaths(const std::string& serverRoot, const std::string& reqPath) {
	std::vector<std::string> rootTokens = split(serverRoot, '/');
	std::vector<std::string> pathTokens = split(reqPath, '/');

	// 마지막 공통 요소를 찾습니다.
	int commonIndex = -1;
	for (unsigned long i = rootTokens.size() - 1, j = 0; i >= 0 && j < pathTokens.size(); --i, ++j) {
		if (rootTokens[i] == pathTokens[j]) {
			commonIndex = j;
			break;
		}
	}

	// 공통 요소를 기준으로 최종 경로를 생성합니다.
	std::string result = "";
	for (unsigned long i = 0; i < rootTokens.size() - 1; ++i) {
		result += rootTokens[i] + "/";
	}
	for (unsigned long i = commonIndex; i < pathTokens.size(); ++i) {
		result += pathTokens[i] + (i < pathTokens.size() - 1 ? "/" : "");
	}

	return result;
}

void DeleteResponseBuilder::deleteFile() {
	const std::string locRoot = mergePaths(
		"." + this->_locationConfig.get()->getDirectives(ROOT).asString() + this->_locationConfig.get()->getPath(),
		this->_request.get()->second.getRequestTarget());

	const std::string serverRoot = mergePaths(
		"." + this->_serverConfig.get()->getDirectives(ROOT).asString() + this->_locationConfig.get()->getPath(),
		this->_request.get()->second.getRequestTarget());

	struct stat fileInfo;
	if (stat(locRoot.c_str(), &fileInfo) == 0) {
		if (S_ISDIR(fileInfo.st_mode)) {
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
				new ErrorResponseBuilder(400, this->_sharedData, this->_serverConfig, this->_locationConfig));
		}
		if (unlink(locRoot.c_str()) == -1) {
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
				new ErrorResponseBuilder(500, this->_sharedData, this->_serverConfig, this->_locationConfig));
		}
	} else if (stat(serverRoot.c_str(), &fileInfo) == 0) {
		if (S_ISDIR(fileInfo.st_mode)) {
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
				new ErrorResponseBuilder(400, this->_sharedData, this->_serverConfig, this->_locationConfig));
		}
		if (unlink(serverRoot.c_str()) == -1) {
			throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
				new ErrorResponseBuilder(500, this->_sharedData, this->_serverConfig, this->_locationConfig));
		}
	}
}

reactor::sharedData_t DeleteResponseBuilder::getProduct() {
	return this->_sharedData;
}

void DeleteResponseBuilder::setStartLine() {
	this->_response.setStartLine(DefaultResponseBuilder::getInstance()->setDefaultStartLine(200));
}

void DeleteResponseBuilder::setHeader() {
	std::map<std::string, std::string> headers =
		DefaultResponseBuilder::getInstance()->setDefaultHeader(this->_serverConfig);
	this->_response.setHeaders(headers);
}

bool DeleteResponseBuilder::setBody() {
	this->_response.setBody("delete success");
	return true;
}

bool DeleteResponseBuilder::build() {
	this->deleteFile();
	this->setStartLine();
	this->setHeader();
	this->setBody();

	const std::string raw = this->_response.getRawStr();
	this->_sharedData.get()->getBuffer().insert(this->_sharedData.get()->getBuffer().end(), raw.begin(), raw.end());
	return true;
}

void DeleteResponseBuilder::reset() {
	this->_response = HttpMessage();
}

void DeleteResponseBuilder::prepare() {
	this->build();
	this->reset();
}
