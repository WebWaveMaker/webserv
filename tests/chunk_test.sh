#!/bin/bash
curl -X $1 $2 \
-H "Transfer-Encoding: chunked" \
-H "Content-Type: text/plain" \
--data-raw "5\r\nHello\r\n6\r\n World\r\n0\r\n\r\n"
