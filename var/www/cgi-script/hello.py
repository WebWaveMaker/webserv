#!/usr/bin/python
# -*- coding: utf-8 -*-

import os

# HTTP 헤더
print("Status: 200 OK\r\nContent-type: text/html\r\n\r\n")

# HTML 문서 출력
print("""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login Page</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }

        .login-container {
            background-color: #fff;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);
            width: 300px;
            text-align: center;
        }

        input[type="text"],
        input[type="password"],
        button {
            width: 100%;
            margin-bottom: 10px;
            padding: 10px;
            border-radius: 3px;
            border: 1px solid #ccc;
            box-sizing: border-box;
        }

        button {
            background-color: #007bff;
            color: #fff;
            cursor: pointer;
        }

        button:hover {
            background-color: #0056b3;
        }

        .signup-link {
            display: inline-block;
            text-decoration: none;
            color: #007bff;
            background-color: transparent;
            border: 1px solid #007bff;
            transition: background-color 0.3s ease;
            padding: 8px 20px;
        }

        .signup-link:hover {
            background-color: #007bff;
            color: #fff;
        }
    </style>
</head>
<body>
    <div class="login-container">
        <h2>Login</h2>
""")

# 환경 변수에서 username과 password 가져와서 출력
username = os.environ.get('X_username')
password = os.environ.get('X_password')

if username and password:
    print("<p>Username: %s</p>" % username)
    print("<p>Password: %s</p>" % password)
else:
    print("<p>No username or password provided</p>")

# HTML 문서 마무리
print("""
    </div>
</body>
</html>
""")
