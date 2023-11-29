#!/bin/bash

# 데이터를 200바이트씩 전송할 파일 생성
echo "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." > data.txt


# 파일 크기 확인
filesize=$(wc -c < data.txt)

# chunked 전송을 위해 curl 사용
curl -X POST -H "Transfer-Encoding: chunked" --data-binary @data.txt http://localhost:1114/post_body --limit-rate 100 --header "Expect:" -v
