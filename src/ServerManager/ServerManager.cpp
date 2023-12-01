#include "ServerManager.hpp"
#include "Dispatcher.hpp"
#include "Server.hpp"

ServerManager::ServerManager() : _configParser(), _servers() {}

void ServerManager::init(int ac, char**& av, char**& envp) {
	if (2 < ac || ac < 1)
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Usage: ./webserv [config_file] or just ./webserv");
	const std::string path = (ac == 1) ? DEFAULT_PATH : av[1];
	this->_envp = envp;
	this->_serverConfigs = this->_configParser.parse(path);
	try {
		this->registerTimeoutEvent();
		this->handleSigPipe();
		this->createServer(this->_serverConfigs);
	} catch (std::exception& e) {
		throw e;
	}
}

void ServerManager::handleSigPipe() {
	signal(SIGPIPE, SIG_IGN);
};

void ServerManager::createServer(config_t& ServerConfigs) {
	std::set<unsigned int> usePort;
	try {
		for (config_t::iterator it = ServerConfigs.begin(); it != ServerConfigs.end(); ++it) {
			unsigned int port = it->get()->getDirectives(LISTEN).asUint();
			if (usePort.find(port) == usePort.end()) {
				usePort.insert(port);
				Server* server = new Server(*it);
				std::map<int, Server*>::iterator sIt = this->_servers.find(server->getFd());

				if (sIt == this->_servers.end()) {
					this->_servers[server->getFd()] = server;
				} else {
					delete server;
				}
			}
		}
		usePort.clear();
	} catch (std::exception& e) {
		throw e;
	}
}

void ServerManager::createClient(int serverFd, int clientFd, struct sockaddr_in& clientAddr) {
	this->getServer(serverFd)->createClient(clientFd, clientAddr);
}

void ServerManager::eraseClient(fd_t fd) {
	for (std::map<int, Server*>::iterator serverIt = this->_servers.begin(); serverIt != this->_servers.end();
		 ++serverIt) {
		if (serverIt->second->hasClient(fd)) {
			serverIt->second->eraseClient(fd);
			break;
		}
	}
}

Server* ServerManager::getServer(int serverFD) const {
	std::map<int, Server*>::const_iterator it = this->_servers.find(serverFD);

	if (it != this->_servers.end()) {
		return (it->second);
	}
	return NULL;
}

config_t ServerManager::getServerConfigs() const {
	return (this->_serverConfigs);
}

utils::shared_ptr<ServerConfig> ServerManager::getServerConfig(const int clientFd, std::string host) const {
	std::string portStr = "80";
	if (host.empty())
		return utils::shared_ptr<ServerConfig>();
	size_t colPos = host.find(':');
	if (colPos != std::string::npos) {
		portStr = host.substr(colPos + 1);
		host.erase(colPos);
	}
	unsigned int port = utils::stoui(portStr);
	unsigned int mainPort = 0;
	std::string mainName;

	for (std::map<int, Server*>::const_iterator serverIt = this->_servers.begin(); serverIt != this->_servers.end();
		 ++serverIt) {
		if (serverIt->second->hasClient(clientFd)) {
			mainPort = serverIt->second->getPort();
			mainName = serverIt->second->getServerName();
			break;
		}
	}
	if (port != mainPort)
		return utils::shared_ptr<ServerConfig>();

	if (port == mainPort && host.compare(mainName) == 0)
		return this->getServerDefaultConfig(clientFd);

	for (config_t::const_iterator it = this->_serverConfigs.begin(); it != this->_serverConfigs.end(); ++it) {
		if (it->get()->getDirectives(LISTEN).asUint() == mainPort &&
			it->get()->getDirectives(SERVER_NAME).asString().compare(host) == 0) {
			return *it;
		}
	}
	return this->getServerDefaultConfig(clientFd);
}

utils::shared_ptr<ServerConfig> ServerManager::getServerDefaultConfig(const int clientFd) const {
	for (std::map<int, Server*>::const_iterator serverIt = this->_servers.begin(); serverIt != this->_servers.end();
		 ++serverIt) {
		if (serverIt->second->hasClient(clientFd)) {
			return (serverIt->second->getConfig());
		}
	}
	return u::shared_ptr<ServerConfig>();
}

ServerManager::~ServerManager() {
	for (std::map<int, Server*>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
		delete it->second;
	this->_servers.clear();
}

void ServerManager::registerReadEvent(fd_t fd) {
	reactor::Dispatcher::getInstance()->registerExeHandler<reactor::ServerAcceptHandlerFactory>(
		reactor::sharedData_t(new reactor::SharedData(fd, EVENT_READ, std::vector<char>())));
}

void ServerManager::registerTimeoutEvent() {
	reactor::sharedData_t timersharedData =
		utils::shared_ptr<reactor::SharedData>(new reactor::SharedData(4242, EVENT_TIMER, std::vector<char>()));
	timersharedData->setReadByte(this->_serverConfigs[0]->getDirectives(KEEPALIVE_TIMEOUT).asUint() * 500);
	reactor::Dispatcher::getInstance()->registerExeHandler<reactor::TimeoutHandlerFactory>(timersharedData);
}

utils::shared_ptr<std::vector<fd_t> > ServerManager::getClientFds() {
	utils::shared_ptr<std::vector<fd_t> > clientFds = utils::shared_ptr<std::vector<fd_t> >(new std::vector<fd_t>);

	for (std::map<int, Server*>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it) {
		u::shared_ptr<std::vector<fd_t> > serverClients = it->second->getClientFds();
		std::copy(serverClients->begin(), serverClients->end(), std::back_inserter(*(clientFds.get())));
	}
	return (clientFds);
}

char** ServerManager::getEnvp() const {
	return (this->_envp);
}

std::string ServerManager::getClientIp(fd_t fd) {
	for (std::map<int, Server*>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it) {
		if (it->second->hasClient(fd)) {
			return std::string(it->second->getClientIP(fd));
		}
	}
	return std::string("");
}

std::string ServerManager::findLocationBlock(const request_t& request,
											 const utils::shared_ptr<ServerConfig>& serverConfig) {
	std::string requestTarget = request->second.getRequestTarget();

	size_t dotPos = requestTarget.find('.');
	if (dotPos == std::string::npos)
		return requestTarget;
	size_t slashPos = requestTarget.find('/', dotPos);
	if (slashPos == std::string::npos) {
		requestTarget.erase(0, dotPos);
		if (serverConfig->getLocation("/" + requestTarget + "/").get() == NULL)
			return request->second.getRequestTarget();
		std::vector<HttpMethods> methods =
			serverConfig->getLocation("/" + requestTarget + "/")->getDirectives(LIMIT_EXCEPT).asMedVec();
		for (std::vector<HttpMethods>::iterator it = methods.begin(); it != methods.end(); ++it) {
			if (*it == request->second.getMethod())
				return "/" + requestTarget;
		}
		return request->second.getRequestTarget();
	}
	requestTarget.erase(slashPos);
	requestTarget.erase(0, dotPos);
	if (serverConfig->getLocation("/" + requestTarget + "/").get() == NULL)
		return request->second.getRequestTarget();
	std::vector<HttpMethods> methods =
		serverConfig->getLocation("/" + requestTarget + "/")->getDirectives(LIMIT_EXCEPT).asMedVec();
	for (std::vector<HttpMethods>::iterator it = methods.begin(); it != methods.end(); ++it) {
		if (*it == request->second.getMethod())
			return "/" + requestTarget;
	}
	return request->second.getRequestTarget();
}
