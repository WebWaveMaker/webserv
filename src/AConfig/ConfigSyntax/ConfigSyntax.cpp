#include "ConfigSyntax.hpp"

bool ConfigSyntax::isSlashFrontAndBack(const std::string& str) {
	if (str[0] == '/' && str[str.size() - 1] == '/') {
		return true;
	}
	return false;
}

bool ConfigSyntax::isSlashFront(const std::string& str) {
	if (str[0] == '/') {
		return true;
	}
	return false;
}

void ConfigSyntax::includeSyntax(const std::vector<std::string>& values) {
	for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it) {
		if (ConfigSyntax::isSlashFront(*it) == false) {
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
										  "The syntax of the directive's parameter is incorrect: the directive is "
										  "[include] and the parameter is " +
											  *it);
		}
	}
}

void ConfigSyntax::indexSyntax(const std::vector<std::string>& values) {
	for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it) {
		if (ConfigSyntax::isSlashFront(*it) == false) {
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
										  "The syntax of the directive's parameter is incorrect: the directive is "
										  "[index] and the parameter is " +
											  *it);
		}
	}
}
void ConfigSyntax::cgiIndexSyntax(const std::vector<std::string>& values) {
	// try {
	// 	std::stoi(values[0]);
	// } catch (std::exception& e) {
	// 	throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
	// 								  "The syntax of the directive's parameter is incorrect: the directive is "
	// 								  "[cgi_index] and the parameter is " +
	// 									  values[0]);
	// }
	for (std::vector<std::string>::const_iterator it = values.begin() + 1; it != values.end(); ++it) {
		if (ConfigSyntax::isSlashFront(*it) == false) {
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
										  "The syntax of the directive's parameter is incorrect: the directive is "
										  "[cgi_index] and the parameter is " +
											  *it);
		}
	}
}

void ConfigSyntax::returnSyntax(const std::vector<std::string>& values) {
	try {
		std::stoi(values[0]);
	} catch (std::exception& e) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
									  "The syntax of the directive's parameter is incorrect: the directive is "
									  "[return] and the parameter is " +
										  values[0]);
	}
	for (std::vector<std::string>::const_iterator it = values.begin() + 1; it != values.end(); ++it) {
		if (ConfigSyntax::isSlashFront(*it) == false) {
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
										  "The syntax of the directive's parameter is incorrect: the directive is "
										  "[return] and the parameter is " +
											  *it);
		}
	}
}

void ConfigSyntax::rootSyntax(const std::vector<std::string>& values) {
	if (ConfigSyntax::isSlashFrontAndBack(values[0]) == false) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
									  "The syntax of the directive's parameter is incorrect: the directive is "
									  "[root] and the parameter is " +
										  values[0]);
	}
}

void ConfigSyntax::errorPageSyntax(const std::vector<std::string>& values) {
	for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end() - 1; ++it) {
		try {
			std::stoi(*it);
		} catch (std::exception& e) {
			throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
										  "The syntax of the directive's parameter is incorrect: the directive is "
										  "[error_page] and the parameter is " +
											  *it);
		}
	}
	if (ConfigSyntax::isSlashFront(values[values.size() - 1]) == false) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
									  "The syntax of the directive's parameter is incorrect: the directive is "
									  "[return] and the parameter is " +
										  values[values.size() - 1]);
	}
}

void ConfigSyntax::errorLogSyntax(const std::vector<std::string>& values) {
	if (values.size() != 2) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
									  "The syntax of the directive's parameter is incorrect: the directive is "
									  "[error_log]");
	}
	if (ConfigSyntax::isSlashFront(values[0]) == false) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
									  "The syntax of the directive's parameter is incorrect: the directive is "
									  "[error_log] and the parameter is " +
										  values[0]);
	}
	if (values[1] == "debug" || values[1] == "info" || values[1] == "warn" || values[1] == "error") {
		return;
	}
	throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
								  "The syntax of the directive's parameter is incorrect: the directive is "
								  "[error_log]");
}

void ConfigSyntax::checkSyntax(const std::string& directive, const std::vector<std::string>& values) {
	if (directive == "index") {
		ConfigSyntax::indexSyntax(values);
	} else if (directive == "cgi_index") {
		ConfigSyntax::cgiIndexSyntax(values);
	} else if (directive == "return") {
		ConfigSyntax::returnSyntax(values);
	} else if (directive == "root") {
		ConfigSyntax::rootSyntax(values);
	} else if (directive == "error_page") {
		ConfigSyntax::errorPageSyntax(values);
	} else if (directive == "error_log") {
		ConfigSyntax::errorLogSyntax(values);
	}
}

void ConfigSyntax::locationSyntax(const std::string& directive) {
	if (ConfigSyntax::isSlashFrontAndBack(directive) == false) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__,
									  "The syntax of the directive's parameter is incorrect: the directive is "
									  "[location] and the parameter is " +
										  directive);
	}
}