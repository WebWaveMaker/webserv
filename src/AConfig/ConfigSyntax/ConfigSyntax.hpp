#pragma once
#ifndef CONFIGSYNTAX_HPP
#define CONFIGSYNTAX_HPP

#include "ConfigSyntax.h"

class ConfigSyntax {
   public:
	static bool isSlashFrontAndBack(const std::string& str);
	static bool isSlashFront(const std::string& str);
	static void includeSyntax(const std::vector<std::string>& values);
	static void indexSyntax(const std::vector<std::string>& values);
	static void cgiIndexSyntax(const std::vector<std::string>& values);
	static void returnSyntax(const std::vector<std::string>& values);
	static void rootSyntax(const std::vector<std::string>& values);
	static void errorPageSyntax(const std::vector<std::string>& values);
	static void errorLogSyntax(const std::vector<std::string>& values);
	static void checkSyntax(const std::string& directive, const std::vector<std::string>& values);
	static void locationSyntax(const std::string& directive);
};

#endif	// CONFIGSYNTAX_HPP