#include "CgiExecuteHandler.hpp"
#include "Dispatcher.hpp"

namespace reactor {
	CgiExecuteHandler::CgiExecuteHandler(sharedData_t& sharedData, va_list args)
		: AExeHandler(sharedData), _request(*va_arg(args, request_t*)) {
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, this->_sv) < 0) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
			throw;
		}
		if (fcntl(this->_sv[0], F_SETFL, O_NONBLOCK) < 0 || fcntl(this->_sv[1], F_SETFL, O_NONBLOCK) < 0) {
			ErrorLogger::systemCallError(__FILE__, __LINE__, __func__);
			throw;
		}
		this->setHandler(this->_sv[0]);

		va_end(args);
	}

	bool CgiExecuteHandler::validCgiPath() {
		const std::string locPath = "." + this->_locationConfig.get()->getDirectives(ROOT).asString();
		const std::string serverPath = "." + this->_serverConfig.get()->getDirectives(ROOT).asString();
		const std::vector<std::string> cgiIndex = "." + this->_locationConfig.get()->getDirectives(CGI_INDEX)
	}

}  // namespace reactor
