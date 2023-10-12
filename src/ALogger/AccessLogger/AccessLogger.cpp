#include "AccessLogger.hpp"

AccessLogger::AccessLogger(const int fd) : ALogger(fd) {}
AccessLogger::AccessLogger(const AccessLogger& obj) : ALogger(obj) {}
AccessLogger::~AccessLogger() {}
AccessLogger& AccessLogger::operator=(const AccessLogger& obj) {
	if (this == &obj)
		return *this;
	return *this;
}

std::string AccessLogger::getHttpMethodStr(const enum HttpMethod& method) {
	switch (method) {
		case GET:
			return "GET ";
		case POST:
			return "POST ";
		case DELETE:
			return "DELETE ";
		case PUT:
			return "PUT ";

		default:
			break;
	}
}

std::string AccessLogger::makeLogMsg(const std::string& msg, const std::string& func, void* arg) {
	const Client* client = static_cast<Client*>(arg);

	return client->_clientAddrStr + this->getCurTime(logTimeFormat::accessTimeFormat) + func + " " + msg + "\n";
}

/**
 * @brief 
 * // log("msg", __func__, GET, &Client);
 * @output // remote_addr [time] method msg ... etc....
 * 
 * @param msg 
 * @param func
 * @param enum__
 * @param arg
 */
void AccessLogger::log(const std::string& msg, const char* func, const int enum__, void* arg) {
	const std::string methodStr = this->getHttpMethodStr(static_cast<HttpMethod>(enum__));
	const std::string buf = this->makeLogMsg(methodStr + msg, func, arg);

	if (write(this->fd_, buf.c_str(), buf.size()) == -1)
		ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
}