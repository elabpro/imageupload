package main

import (
  "log"
  "net/http"
  "math/rand"
  "strconv"
  "encoding/json"
  "github.com/gorilla/mux"
)

func main(){
  r := mux.NewRouter()
  r.HandleFunc("/api/images",getImages).Methods("GET")
  r.HandleFunc("/api/images/{id}",getImage).Methods("GET")
  r.HandelFunc("/api/images",createImages).Method("POST")
  log.Fatal(http.ListenAndServer(":8080",r))
}
