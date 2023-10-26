#include "RequestParser.hpp"

RequestParser::RequestParser(ServerConfig& serverConfig)
	: IParser(), _msgs(), _curMsg(u::nullptr_t), _buf(), _serverConfig(serverConfig) {
	_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
		new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
	_curMsg = &_msgs.back();
}

RequestParser::~RequestParser() {}

enum RequestParserState RequestParser::getState(void) const {
	return this->_msgs.empty() ? RESOLVE : PENDING;
}

std::string RequestParser::findAndSubstr(std::string& buf, std::string delim) {
	std::string::size_type size = buf.find(delim);
	if (size == std::string::npos) {
		this->_buf += buf;
		buf.clear();
		return "";
	}
	if (this->_buf.size()) {
		buf = this->_buf + buf;
		size += this->_buf.size();
		this->_buf.clear();
	}
	std::string str = buf.substr(0, size);
	buf = buf.substr(size + delim.size());
	return str;
}

request_t RequestParser::get(void) {
	return this->pop();
}

request_t RequestParser::pop(void) {
	request_t elem;
	if (this->_msgs.empty())
		return elem;
	if (this->_msgs.front().get()->first == DONE || this->_msgs.front().get()->first == ERROR) {
		elem = this->_msgs.front();
		this->_msgs.pop();
	}
	return elem;
}

bool RequestParser::parseStartLine(std::string& buf) {
	std::string str = this->findAndSubstr(buf, CRLF);
	if (str.size() == 0)
		return false;
	std::stringstream ss(str);
	std::string startLine[3];

	ss >> startLine[0] >> startLine[1] >> startLine[2];
	if (startLine[0].empty() || startLine[1].empty() || startLine[2].empty()) {
		_curMsg->get()->first = ERROR;
		return false;
	}

	_curMsg->get()->second.setStartLine(startLine);
	_curMsg->get()->first = HEADER;
	return true;
}

bool RequestParser::parseHeader(std::string& buf) {
	std::string str = this->findAndSubstr(buf, RNRN);
	if (str.size() == 0)
		return false;
	str += CRLF;
	std::vector<std::string> headerFields = utils::split(str, CRLF);
	std::map<std::string, std::string> headers;

	for (std::vector<std::string>::iterator it = headerFields.begin(); it != headerFields.end(); ++it) {
		const std::string::size_type colonPos = (*it).find(':');
		if (colonPos == std::string::npos)
			continue;
		const std::string key = (*it).substr(0, colonPos);
		const std::string val = utils::trim((*it).substr(colonPos + 1));

		headers[key] = val;	 // 중복은 어떻게 처리? 우선은 overwrite
	}
	if (_curMsg->get()->second.getMethod() == "POST" && headers.count("Content-Length") == 0) {
		_curMsg->get()->first = ERROR;
		return false;
	}

	_curMsg->get()->second.setHeaders(headers);
	_curMsg->get()->first = BODY;
	return true;
}

bool RequestParser::parserBody(std::string& buf) {
	_curMsg->get()->first = DONE;
	const std::map<std::string, std::string>& headers = _curMsg->get()->second.getHeaders();

	if (headers.count("Content-Length") == 0)
		return true;
	const unsigned int contentLength = utils::stoui(headers.at("Content-Length"));
	const unsigned int bodyLimit = _serverConfig.getDirectives(CLIENT_MAX_BODY_SIZE).asUint();

	if (contentLength > bodyLimit) {
		_curMsg->get()->first = ERROR;
		return false;
	};
	try {
		const std::string str = buf.substr(0, contentLength);
		buf = buf.substr(contentLength);
		_curMsg->get()->second.setBody(str);
	} catch (const std::out_of_range& ex) {
		ErrorLogger::log(__FILE__, __LINE__, __func__, "content-length too large compared of body");
		_curMsg->get()->first = ERROR;
		return false;
	}
	return true;
}

// 비 정상적일때 에러처리를 어떻게 할 것인가.
request_t RequestParser::parse(const std::string& content) {
	if (content.size() == 0)
		return this->pop();
	std::string buf(content);

	while (buf.empty() == false) {
		switch (this->_msgs.back().get()->first) {
			case START_LINE:
				this->parseStartLine(buf);
				break;
			case HEADER:
				this->parseHeader(buf);
				break;
			case BODY:
				this->parserBody(buf);
				break;
			case ERROR:
				buf.clear();
			default:
				break;
		}
		if (this->_msgs.back().get()->first == DONE && buf.empty() == false) {
			_msgs.push(utils::shared_ptr<std::pair<enum HttpMessageState, HttpMessage> >(
				new std::pair<enum HttpMessageState, HttpMessage>(START_LINE, HttpMessage())));
			_curMsg = &_msgs.back();
		}
	}
	if (this->_curMsg->get()->first == BODY && this->_curMsg->get()->second.getHeaders().count("Content-Length") == 0)
		this->_curMsg->get()->first = DONE;
	return this->pop();
}
