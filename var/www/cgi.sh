#!/bin/bash

# HTTP 상태 코드와 컨텐트 타입을 출력
echo "Status: 200 OK"
echo "Content-type: text/html"
echo ""

# HTML 문서 시작
echo "<html><head><title>CGI Test</title></head><body>"
echo "<h1>CGI Test Page</h1>"
echo "<p>This is a test page for CGI scripts</p>"

# 환경 변수 출력
echo "<h2>Environment Variables</h2>"
echo "<pre>"
env
echo "</pre>"

# HTML 문서 종료
echo "</body></html>"
