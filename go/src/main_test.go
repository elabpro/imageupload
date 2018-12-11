package main

import (
	"os"
	"strconv"
	"testing"
)

func TestDownloadFile(t *testing.T) {
	filename := "test.image"
	err := DownloadFile(filename, "http://yandex.ru/favicon.ico")
	if err != nil {
		t.Fatalf("Can't download file")
	} else {
		if _, err := os.Stat(filename); os.IsNotExist(err) {
			t.Fatal("File is not exists")
		} else {
			os.Remove(filename)
		}
	}
}

func TestCheck(t *testing.T) {
	config = LoadConfiguration("config_test.json")
	var expectedValue int64
	expectedValue = 100
	filename := config.ImagesDir + "/" + strconv.FormatInt(expectedValue, 10)
	_, err := os.Create(filename)
	if err != nil {
		t.Fatal("Can't create file for a test")
	}
	maxId := check()
	if maxId < expectedValue {
		t.Fatal("Wrong maxId:" + strconv.FormatInt(maxId, 10))
	}
	os.Remove(filename)
}
