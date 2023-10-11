#pragma once
#include "IParser.h"

template <typename T>
class IParser {
   public:
	/**
     * @brief 가상 소멸자 
     */
	virtual ~IParser() {}

	/**
     * @brief 파싱 메소드
     * @param content 파싱할 내용
     * @param method 파싱된 결과 
     * @return 파싱 성공 여부 
     */
	virtual bool parse(const std::string& content, T& method) = 0;

   protected:
	/**
     * @brief 보호된 생성자 
     */
	IParser() {}
};
