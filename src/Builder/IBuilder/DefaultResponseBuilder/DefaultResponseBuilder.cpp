#include "DefaultResponseBuilder.hpp"

void DefaultResponseBuilder::initHttpStatusCode() {
	_httpStatusCodes["200"] = "OK";
	_httpStatusCodes["201"] = "Created";
	_httpStatusCodes["202"] = "Accepted";
	_httpStatusCodes["204"] = "No Content";
	_httpStatusCodes["206"] = "Partial Content";

	_httpStatusCodes["300"] = "Multiple Choices";
	_httpStatusCodes["301"] = "Moved Permanently";
	_httpStatusCodes["302"] = "Found";
	_httpStatusCodes["303"] = "See Other";
	_httpStatusCodes["304"] = "Not Modified";
	_httpStatusCodes["307"] = "Temporary Redirect";
	_httpStatusCodes["308"] = "Permanent Redirect";

	_httpStatusCodes["400"] = "Bad Request";
	_httpStatusCodes["401"] = "Unauthorized";
	_httpStatusCodes["403"] = "Forbidden";
	_httpStatusCodes["404"] = "Not Found";
	_httpStatusCodes["405"] = "Method Not Allowed";
	_httpStatusCodes["406"] = "Not Acceptable";
	_httpStatusCodes["408"] = "Request Timeout";
	_httpStatusCodes["409"] = "Conflict";
	_httpStatusCodes["410"] = "Gone";
	_httpStatusCodes["415"] = "Unsupported Media Type";
	_httpStatusCodes["429"] = "Too Many Requests";

	_httpStatusCodes["500"] = "Internal Server Error";
	_httpStatusCodes["501"] = "Not Implemented";
	_httpStatusCodes["502"] = "Bad Gateway";
	_httpStatusCodes["503"] = "Service Unavailable";
	_httpStatusCodes["504"] = "Gateway Timeout";
	_httpStatusCodes["505"] = "HTTP Version Not Supported";
}

DefaultResponseBuilder::DefaultResponseBuilder() : _httpStatusCodes() {
	initHttpStatusCode();
}
DefaultResponseBuilder::~DefaultResponseBuilder() {}

std::vector<std::string> DefaultResponseBuilder::setDefaultStartLine(const int statusCode) {
	const std::string str = utils::itos(statusCode);
	std::vector<std::string> startLine(3);
	startLine[0] = "HTTP/1.1";
	startLine[1] = str;
	startLine[2] = this->_httpStatusCodes[str];
	return startLine;
}

std::vector<std::string> DefaultResponseBuilder::setDefaultStartLine(
	int statusCode, const std::string extension, const utils::shared_ptr<ServerConfig>& serverConfig) {
	if (serverConfig->hasMimeTypes(extension) == false)
		statusCode = UNSUPPORTED_MEDIA_TYPE;
	const std::string str = utils::itos(statusCode);
	std::vector<std::string> startLine(3);
	startLine[0] = "HTTP/1.1";
	startLine[1] = str;
	startLine[2] = this->_httpStatusCodes[str];
	return startLine;
}

std::map<std::string, std::string> DefaultResponseBuilder::setDefaultHeader(
	const utils::shared_ptr<ServerConfig>& serverConfig, const std::string& path) {
	std::map<std::string, std::string> headers;

	headers["Server"] = serverConfig->getDirectives(SERVER_NAME).asString();
	headers["Date"] = utils::getCurTime(logTimeFormat::GMTtimeFormat);
	headers["Content-Type"] = serverConfig->getMimeTypes(path.substr(path.find_last_of('.') + 1));

	return headers;
}

std::map<std::string, std::string> DefaultResponseBuilder::setDefaultHeader(
	const utils::shared_ptr<ServerConfig>& serverConfig) {
	std::map<std::string, std::string> headers;

	headers["Server"] = serverConfig->getDirectives(SERVER_NAME).asString();
	headers["Date"] = utils::getCurTime(logTimeFormat::GMTtimeFormat);

	return headers;
}