FROM golang:1.10-alpine

#ENV http_proxy http://172.17.0.1:5865
#ENV https_proxy http://172.17.0.1:5865

#COPY apt.conf /etc/apt/apt.conf

RUN apk update && apk add make git curl net-tools imagemagick

CMD exec go get github.com/go-redis/redis
CMD exec go get github.com/gorilla/mux

EXPOSE 8080

#COPY docker-entrypoint.sh /usr/local/bin/

#ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]

