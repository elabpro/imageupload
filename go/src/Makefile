build: clean
	go build main.go
clean:
	rm -f main
prepare:
	go get github.com/go-redis/redis
	go get github.com/gorilla/mux
docker: prepare build
	echo "Starting API server"
	./main

