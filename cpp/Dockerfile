FROM alphine:latest

#ENV http_proxy http://172.17.0.1:5865
#ENV https_proxy http://172.17.0.1:5865
#COPY apt.conf /etc/apt/apt.conf

RUN apk update && apk add curl imagemagick
RUN apk add libboost-system-dev libboost-filesystem-dev make g++ 

EXPOSE 8080


