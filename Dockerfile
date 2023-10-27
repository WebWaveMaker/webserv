FROM ubuntu:latest

RUN apt update && apt install g++ make libkqueue-dev -y

WORKDIR /app

COPY . .

CMD [ "sh", "-c", "make fclean && make check && make fclean" ]