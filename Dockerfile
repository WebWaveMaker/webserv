FROM alpine:latest

RUN apk update && apk add make g++

WORKDIR /app

COPY . .

CMD [ "sh", "-c", "make fclean && make && make fclean" ]