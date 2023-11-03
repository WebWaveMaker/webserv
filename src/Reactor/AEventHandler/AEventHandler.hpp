#pragma once
#ifndef AEVENTHANDLER_HPP
#define AEVENTHANDLER_HPP

#include "AEventHandler.h"

namespace reactor {

	class AEventHandler {
	   private:
	   protected:
		const handle_t _fd;
		const utils::shared_ptr<AccessLogger> _accessLogger;
		const utils::shared_ptr<ErrorLogger> _errorLogger;
		utils::shared_ptr<std::vector<char> > _buf;

	   public:
		AEventHandler(const handle_t fd, const utils::shared_ptr<AccessLogger>& accessLogger,
					  const utils::shared_ptr<ErrorLogger>& errorLogger)
			: _fd(fd), _accessLogger(accessLogger), _errorLogger(errorLogger), _buf(new std::vector<char>){};
		AEventHandler(const handle_t fd, const utils::shared_ptr<AccessLogger>& accessLogger,
					  const utils::shared_ptr<ErrorLogger>& errorLogger, utils::shared_ptr<std::vector<char> > buf)
			: _fd(fd), _accessLogger(accessLogger), _errorLogger(errorLogger), _buf(buf){};
		virtual ~AEventHandler(){};
		virtual sharedData_t getData() const { return this->_sharedData; };
		virtual handle_t getHandle() const { return this->_sharedData.get()->fd; };
		virtual std::vector<char> getBuffer() const { return this->_sharedData.get()->buffer; };
		virtual enum ASyncState getState() const { return this->_sharedData.get()->state; };
		virtual void setState(const enum AsyncState state) { this->_sharedData.get()->state = state; };
		virtual void handleEvent() = 0;
	};

}  // namespace reactor

#endif
