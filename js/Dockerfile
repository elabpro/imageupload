FROM node:8

#ENV http_proxy http://172.17.0.1:5865
#ENV https_proxy http://172.17.0.1:5865
#COPY apt.conf /etc/apt/apt.conf

RUN apt-get -y update
RUN apt-get -y install curl imagemagick
RUN mkdir /usr/local/imageapi
RUN cd /usr/local/imageapi && npm install express

EXPOSE 8000

