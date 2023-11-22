#!/bin/bash

# HTTP 상태 코드와 컨텐트 타입을 출력
echo -e "Status: 200 OK\r\nContent-type: text/html\r\nContent-Length: 498\r\n"

# HTML 문서 시작
echo -e "<html><head><title>CGI Test</title></head><body>"
echo -e "<h1>CGI Test Page</h1>"
echo -e "<p>This is a test page for CGI scripts</p>"

# 환경 변수 출력
echo -e "<h2>Environment Variables</h2>"
echo -e "<pre>"
/usr/bin/env
echo -e "</pre>"

# HTML 문서 종료
echo -e "</body></html>"
