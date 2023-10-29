/**
 * @file HttpConfig.hpp
 * @brief Defines the HttpConfig class for handling HTTP-specific configurations.
 */

#pragma once
#ifndef HTTPCONFIG_HPP
#define HTTPCONFIG_HPP

#include "HttpConfig.h"

/**
 * @class HttpConfig
 * @brief Concrete implementation of the AConfig interface for HTTP-specific configurations.
 * 
 * This class is responsible for setting, retrieving, and managing HTTP configurations. It inherits
 * from the AConfig interface and implements all its pure virtual methods.
*/
class HttpConfig : public AConfig {
   public:
	/**
     * @brief Default constructor.
     * 
     * Initializes a new HttpConfig object.
     */
	HttpConfig();

	/**
     * @brief Copy constructor.
     * 
     * Creates a new HttpConfig object by copying the contents from another object.
     * 
     * @param other The HttpConfig object to copy from.
     */
	HttpConfig(const HttpConfig& other);

	/**
     * @brief Destructor.
     * 
     * Cleans up the resources.
     */
	~HttpConfig();

	/**
     * @brief Copy assignment operator.
     * 
     * Assigns the state of one HttpConfig object to another.
     * 
     * @param other The HttpConfig object to copy from.
     * @return A reference to the modified object.
     */
	HttpConfig& operator=(const HttpConfig& other);

	/**
     * @brief Sets the directive configuration.
     * 
     * Implements the pure virtual function from AConfig to set HTTP-specific directives.
     * 
     * @param directive The name of the directive.
     * @param values A vector containing the values for the directive.
     */
	virtual void setDirectives(const std::string& directive, const std::vector<std::string>& values);

	/**
     * @brief Sets the error page configuration.
     * 
     * Implements the pure virtual function from AConfig to set HTTP-specific error pages.
     * 
     * @param values A vector containing the values for the error pages.
     */
	virtual void setErrorPage(const std::vector<std::string>& values);

	/**
     * @brief Gets the URL of the error page based on the error code.
     * 
     * Implements the pure virtual function from AConfig to retrieve HTTP-specific error pages.
     * 
     * @param error_code The error code for which to fetch the error page.
     * @return A string containing the URL of the error page.
     */
	virtual std::string getErrorPage(unsigned int error_code) const;

	/**
     * @brief Gets the directive configuration based on the directive method.
     * 
     * Implements the pure virtual function from AConfig to retrieve HTTP-specific directives.
     * 
     * @param method The method for which to fetch the directive.
     * @return A ConfigValue object containing the directive values.
     */
	virtual ConfigValue getDirectives(Directives method) const;
};

#endif	// HTTPCONFIG_HPP
