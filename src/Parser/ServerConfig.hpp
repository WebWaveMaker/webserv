#pragma once
#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP
//
class ServerConfig {
   private:
   public:
	ServerConfig();
	ServerConfig(const ServerConfig& obj);
	~ServerConfig();
	ServerConfig& operator=(const ServerConfig& obj);
};

#endif