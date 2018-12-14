FROM debian

ENV http_proxy http://172.17.0.1:5865
ENV https_proxy http://172.17.0.1:5865
COPY apt.conf /etc/apt/apt.conf

RUN apt-get -y update && apt-get -y install curl imagemagick
RUN apt-get -y install libboost-all-dev make cmake g++ 
RUN apt-get -y install pkg-config
RUN apt-get -y install libfastjson-dev libpthread-stubs0-dev libcurl4-gnutls-dev

RUN mkdir -p /usr/local/imageapi
COPY libs /usr/local/imageapi/libs
COPY nbproject /usr/local/imageapi/nbproject
COPY *.cpp /usr/local/imageapi/
COPY *.h /usr/local/imageapi/
COPY Makefile /usr/local/imageapi/
COPY prepare.sh /usr/local/imageapi/
RUN cd  /usr/local/imageapi/ && sh ./prepare.sh 
RUN cd  /usr/local/imageapi/ && make

EXPOSE 8080


