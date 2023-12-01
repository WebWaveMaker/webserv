#!/usr/bin/python
# -*- coding: utf-8 -*-

import os

# HTTP 헤더
print("Status: 200 OK\r\nContent-type: text/html\r\n\r\n")

# HTML 문서 출력
print("""
<!DOCTYPE html>
<html>
<head>
    <title>CGI Test</title>
</head>
<body>
    <h1>CGI Test Page</h1>
    <p>This is a test page for CGI scripts</p>

    <h2>Environment Variables</h2>
    <pre>
""")

# 환경 변수 출력
for param, value in os.environ.items():
    print("%s: %s<br>" % (param, value))

# HTML 문서 마무리
print("""
    </pre>
</body>
</html>
""")
