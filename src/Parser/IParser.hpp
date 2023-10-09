
#pragma once
#ifndef _IPARSER_HPP
#define _IPARSER_HPP
#include <string>

/**
 * @brief Interface for parsing a string into a given type
 * 
 * @tparam T Type to parse into
 */
template <typename T>
class IParser
{
public:
    // Virtual destructor to ensure derived classes are cleaned up correctly
    virtual ~IParser() {}

    // Unified parse method
    virtual bool Parse(const std::string &content, T &method) = 0;

protected:
    // Protected constructor to prevent instantiation of the interface
    IParser() {}
};

#endif // _IPARSER_HPP