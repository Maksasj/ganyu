FROM ubuntu:latest

RUN apt update

RUN apt install bash -y
RUN apt install libpq-dev -y 
RUN apt install iputils-ping -y

RUN mkdir -p /home
WORKDIR /home
COPY . /home

EXPOSE 80

CMD bash ./run.sh