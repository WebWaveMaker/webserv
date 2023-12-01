#pragma once
#ifndef IPARSER_HPP
#define IPARSER_HPP

#include <string>
template <typename T, typename T2>
class IParser {
   public:
	/**
     * @brief 가상 소멸자 
     */
	virtual ~IParser() {}

	/**
     * @brief 파싱 메소드
     * @param content 파싱할 내용
     * @return 파싱 성공 여부 
     */
	virtual T parse(T2& content) = 0;

   protected:
	/**
     * @brief 보호된 생성자 
     */
	IParser() {}
};

#endif
