#include "AccessLogger.hpp"

AccessLogger::AccessLogger() {}
AccessLogger::AccessLogger(const AccessLogger& obj) {}
AccessLogger::~AccessLogger() {}
AccessLogger& AccessLogger::operator=(const AccessLogger& obj) {}

std::string AccessLogger::getHttpMethodStr(const enum& HttpMethod method) {
	switch (method) {
		case GET:
			return "GET";
		case POST:
			return "POST";
		case DELETE:
			return "DELETE";
		case PUT:
			return "PUT";

		default:
			break;
	}
}

std::string AccessLogger::makeLogMsg(const std::string& msg, const std::string& func) {}

/**
 * @brief 
 * // log("msg", __func__, GET)
 * @output // remote_addr [time] method msg ... etc....
 * 
 * @param msg 
 * @param func
 * @param arg
 */
void AccessLogger::log(const std::string& msg, const char* func, const int arg, void *extra) {
    const std::string methodStr = this->getHttpMethodStr(arg);
    const Client *client = const_cast<Client *>extra;

}