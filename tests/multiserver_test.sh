#!/bin/sh

curl http://localhost:4040/ -H "Host: one:4040" -v

curl http://localhost:4040/ -H "Host: one:1414" -v

curl http://localhost:4040/ -H "Host: one" -v

curl http://localhost:4040/ -H "Host: two:4040" -v

curl http://localhost:4040/ -H "Host: two:1414" -v

curl http://localhost:4040/ -H "Host: two" -v