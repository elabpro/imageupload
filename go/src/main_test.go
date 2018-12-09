package main

import (
    "testing"
    "os"
)

func TestDownloadFile(t *testing.T) {
    filename := "test.image"
    err := DownloadFile(filename,"http://yandex.ru/favicon.ico")
    if err != nil {
        t.Fatalf("Can't download file")
    }else{
        if _, err := os.Stat(filename); os.IsNotExist(err) {
            t.Fatal("File is not exists")
        }else{
            os.Remove(filename)
        }
    }
}

