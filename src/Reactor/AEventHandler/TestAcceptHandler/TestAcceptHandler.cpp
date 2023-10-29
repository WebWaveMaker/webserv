#include "TestAcceptHandler.hpp"

void TestAcceptHandler::printClientInfo(const Client* client) {
	if (client == u::nullptr_t) {
		std::cout << "Client is NULL\n";
		return;
	}
	std::cout << "!!Client Info!!\n";
	std::cout << "clientFd:  " + u::itos(client->getFd()) << std::endl;
	std::cout << "IP Address: " << inet_ntoa(client->getAddr().sin_addr) << std::endl;
	std::cout << "Port: " << ntohs(client->getAddr().sin_port) << std::endl;
}

void TestAcceptHandler::printAllClientInfo(const std::map<int, u::shared_ptr<Client> > clients) {
	for (std::map<int, u::shared_ptr<Client> >::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		TestAcceptHandler::printClientInfo(it->second.get());
	}
}
