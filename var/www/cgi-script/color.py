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
    <title>Color Show</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
        }

        .color-container {
            display: flex;
            flex-direction: column;
            align-items: center;
            margin-left: 20px;
        }

        .color-box {
            width: 200px;
            height: 200px;
            border: 1px solid #000;
            margin: 20px;
        }

        .color-text {
            text-align: center;
            margin-top: 10px;
        }
    </style>
</head>
<body>
    <div class="color-container">
        <h1>Color Display</h1>
""")

# 환경 변수에서 X_color 가져와서 Pantone 색상으로 표시
color_code = os.environ.get('X_color')
if color_code:
    # RGB 색상
    print("<div class='color-box' style='background-color: %s;'></div>" % color_code)
    print("<p class='color-text'>RGB: %s</p>" % color_code)

    # HEX 색상
    try:
        hex_color = '#' + ''.join(['{:02x}'.format(int(channel)) for channel in color_code.split(',')])
        print("<div class='color-box' style='background-color: %s;'></div>" % hex_color)
        print("<p class='color-text'>HEX: %s</p>" % hex_color)
    except ValueError:
        pass

    # Pantone 색상
    # Pantone 컬러를 표시하는 코드는 추가 작업이 필요하며, 보다 복잡합니다.
    # Pantone 코드를 표시하려면 색상 데이터베이스가 필요합니다.

else:
    print("<p>No color provided</p>")

# HTML 문서 마무리
print("""
    </div>
</body>
</html>
""")
