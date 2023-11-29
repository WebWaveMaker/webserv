/**
 * @file AConfig.hpp
 * @brief Declaration for the AConfig class.
 */

#pragma once
#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include "ConfigValue.hpp"	// For ConfigValue class
#include "ErrorLogger.hpp"	// For ErrorLogger class
#include "Parser.h"			// For LogLevels enum
#include "utils.hpp"		// For utils::stringToDecimal() function

#include <map>	   // For std::map
#include <string>  // For std::string
#include <vector>  // For std::vector

/**
 * @class AConfig
 * @brief Abstract base class for different types of server configurations.
 *
 * AConfig provides an interface and some common utilities for handling 
 * server configurations. Derived classes are expected to provide specific 
 * implementations.
 */
class AConfig {
   protected:
	/// Map to hold directives and their respective ConfigValues.
	std::map<Directives, ConfigValue> _directives;

	/// Map to hold error codes and their respective error page URLs.
	std::map<unsigned int, std::string> _errorPages;

	// Member function prototypes for internal utility functions
	ConfigValue addBooleanValue(const std::string& value);
	ConfigValue addUnsignedIntValue(const std::string& value);
	ConfigValue addStringValue(const std::string& value);
	ConfigValue addLogValue(const std::vector<std::string>& value);
	ConfigValue addMedVecValue(const std::vector<HttpMethods>& value);
	ConfigValue addStrVecValue(const std::vector<std::string>& value);

   public:
	/**
	 * @brief Default constructor.
	 */
	AConfig();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~AConfig();

	/**
	 * @brief Copy constructor.
	 * @param other The object to copy.
	 */
	AConfig(const AConfig& other);

	/**
	 * @brief Copy assignment operator.
	 * @param other The object to copy.
	 * @return Reference to *this.
	 */
	AConfig& operator=(const AConfig& other);

	/**
	 * @brief Set the directives in the configuration.
	 * 
	 * This is a pure virtual function and must be implemented by any derived classes.
	 * It sets the directives in the configuration using the given directive name and values.
	 * 
	 * @param directive The name of the directive.
	 * @param values A vector of string values corresponding to the directive.
	 */
	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values) = 0;
	/**
	 * @brief Set the error page configuration.
	 * 
	 * This is a pure virtual function and must be implemented by any derived classes.
	 * It sets the error page configuration using the given values.
	 * 
	 * @param values A vector of string values corresponding to the error page configuration.
	 */
	virtual void setErrorPage(const std::vector<std::string>& values) = 0;
	/**
	 * @brief Retrieve the error page URL based on the error code.
	 * 
	 * This is a pure virtual function and must be implemented by any derived classes.
	 * It retrieves the URL of the error page corresponding to the given error code.
	 * 
	 * @param error_code The HTTP error code.
	 * @return A string containing the URL of the error page.
	 */
	virtual std::string getErrorPage(unsigned int error_code) const = 0;
	/**
	 * @brief Retrieve the directive value based on the method.
	 * 
	 * This is a pure virtual function and must be implemented by any derived classes.
	 * It retrieves the value of a configuration directive based on the given method.
	 * 
	 * @param method The method for which the directive value is queried.
	 * @return A ConfigValue object containing the directive value.
	 */
	virtual ConfigValue getDirectives(Directives method) const = 0;

	/**
	 * @brief Check if the given method is a directive.
	 * 
	 * This function checks if the given method is a directive.
	 * 
	 * @param method The method to check.
	 * @return true if the method is a directive, false otherwise.
	 */
	bool isDirective(Directives method) const;
};

#endif	// ACONFIG_HPP
