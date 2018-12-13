FROM debian

ENV http_proxy http://172.17.0.1:5865
ENV https_proxy http://172.17.0.1:5865
COPY apt.conf /etc/apt/apt.conf

RUN apt-get -y update && apt-get -y install curl imagemagick

EXPOSE 8080


