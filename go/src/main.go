package main

import (
	"bytes"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"github.com/go-redis/redis"
	"github.com/gorilla/mux"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strconv"
)

type Config struct {
	Redis struct {
		Host string `json:"host"`
	} `json:"database"`
	Host          string `json:"host"`
	Port          string `json:"port"`
	ImagesDir     string `json:"imagesDir"`
	ImagesThumDir string `json:"imagesTumbDir"`
}

type FileRecord struct {
	Name    string `json:"name"`
	Content string `json:content`
	URL     string `json:url`
}

//type JSONRequest struct {
//	Files []FileRecord
//}

var config Config
var DBconnection *redis.Client

func LoadConfiguration(file string) (Config) {
	var conf Config
	configFile, err := os.Open(file)
	defer configFile.Close()
	if err != nil {
		return conf
	}
	jsonParser := json.NewDecoder(configFile)
	jsonParser.Decode(&conf)
	return conf
}

/**
 * Getting array of images (JSON)
 * @usage http://localhost:8080/api/images
 */
func getImages(w http.ResponseWriter, r *http.Request) {
	images, err := ioutil.ReadDir(config.ImagesDir)
	if err != nil {
		return
	}
	var res []string
	for _, file := range images {
		if !file.IsDir() {
			res = append(res, file.Name())
		}
	}
	imagesJSON, _ := json.Marshal(res)
	fmt.Fprint(w, string(imagesJSON))
}

/**
 * Getting an image
 * @usage http://localhost:8080/api/images/{id}
 */
func getImage(w http.ResponseWriter, r *http.Request) {
	Filename := config.ImagesDir + "/1.png"
	//Check if file exists and open
	Openfile, err := os.Open(Filename)
	defer Openfile.Close() //Close after function return
	if err != nil {
		//File not found, send 404
		http.Error(w, "File not found.", 404)
		return
	}

	//File is found, create and send the correct headers

	//Get the Content-Type of the file
	//Create a buffer to store the header of the file in
	FileHeader := make([]byte, 512)
	//Copy the headers into the FileHeader buffer
	Openfile.Read(FileHeader)
	//Get content type of file
	FileContentType := http.DetectContentType(FileHeader)

	//Get the file size
	FileStat, _ := Openfile.Stat()                     //Get info from file
	FileSize := strconv.FormatInt(FileStat.Size(), 10) //Get file size as a string
	w.Header().Set("Content-Disposition", "attachment; filename=1.png")
	w.Header().Set("Content-Type", FileContentType)
	w.Header().Set("Content-Length", FileSize)
	//Send the file
	//We read 512 bytes from the file already, so we reset the offset back to 0
	Openfile.Seek(0, 0)
	io.Copy(w, Openfile) //'Copy' the file to the client
}

func createImages(w http.ResponseWriter, r *http.Request) {
	var images []string

	fmt.Println("Trying to upload images")
	reader, err := r.MultipartReader()
	if err != nil {
		// No multipart content
		uploadRequest := make([]FileRecord, 0)
		err = json.NewDecoder(r.Body).Decode(&uploadRequest)
		if err != nil {
			fmt.Println("PANIC:" + err.Error())
		} else {
			for _, file := range uploadRequest {
				if file.Content != "" {
					fileId, err := DBconnection.Incr("counter").Result()
					if err != nil {
						panic(err)
					}
					dst, err := os.Create(config.ImagesDir + "/" + strconv.FormatInt(fileId, 10) + ".png")
					defer dst.Close()
					if err != nil {
						http.Error(w, err.Error(), http.StatusInternalServerError)
						return
					}
					decoded, err := base64.StdEncoding.DecodeString(file.Content)
					if err != nil {
						fmt.Println("decode error:", err)
						return
					} else {
						dst.Write(decoded)
					}
					images = append(images, strconv.FormatInt(fileId, 10))
					fmt.Println("Writing file " + strconv.FormatInt(fileId, 10))
				}
				if file.URL != "" {
					fileId, err := DBconnection.Incr("counter").Result()
					if err != nil {
						panic(err)
					}
					dstFilename := config.ImagesDir + "/" + strconv.FormatInt(fileId, 10) + ".png"
					err = DownloadFile(dstFilename, file.URL)
					if err != nil {
						panic(err)
					} else {
						images = append(images, strconv.FormatInt(fileId, 10))
						fmt.Println("Writing url to file " + dstFilename)
					}
				}
			}
		}
	} else {
		// Parsing multipart files
		for {
			part, err_part := reader.NextPart()
			if err_part == io.EOF {
				break
			}
			if part.FileName() != "" {
				fileId, err := DBconnection.Incr("counter").Result()
				if err != nil {
					panic(err)
				}
				dstFilename := config.ImagesDir + "/" + strconv.FormatInt(fileId, 10) + ".png"
				dst, err := os.Create(dstFilename)
				defer dst.Close()
				if err != nil {
					http.Error(w, err.Error(), http.StatusInternalServerError)
					return
				}
				if _, err := io.Copy(dst, part); err != nil {
					http.Error(w, err.Error(), http.StatusInternalServerError)
					return
				}
				images = append(images, strconv.FormatInt(fileId, 10))
				fmt.Println("Writing mp file " + dstFilename)
			}
			if part.FormName() == "url" {
				buf := new(bytes.Buffer)
				buf.ReadFrom(part)
				fileId, err := DBconnection.Incr("counter").Result()
				if err != nil {
					panic(err)
				}
				dstFilename := config.ImagesDir + "/" + strconv.FormatInt(fileId, 10) + ".png"
				err = DownloadFile(dstFilename, buf.String())
				if err != nil {
					panic(err)
				} else {
					images = append(images, strconv.FormatInt(fileId, 10))
					fmt.Println("Writing mp url to file " + dstFilename)
				}
			}
		}
	}
	response, _ := json.Marshal(images)
	fmt.Println("RESPONSE:" + string(response))
	fmt.Fprint(w, string(response))
}

func DBConnect(host string) {
	DBconnection = redis.NewClient(&redis.Options{
		Addr:     host + ":6379",
		Password: "", // no password set
		DB:       0,  // use default DB
	})
	_, err := DBconnection.Ping().Result()
	if err != nil {
		log.Fatal("Can't connect to Redis (" + host + ")")
	} else {
		fmt.Println("Connection to Redis server (" + host + ") is established")
	}
}

func DownloadFile(filepath string, url string) error {

	// Create the file
	out, err := os.Create(filepath)
	if err != nil {
		return err
	}
	defer out.Close()

	// Get the data
	resp, err := http.Get(url)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	// Write the body to file
	_, err = io.Copy(out, resp.Body)
	if err != nil {
		return err
	}

	return nil
}

func main() {
	config = LoadConfiguration("config.json")
	DBConnect(config.Redis.Host)
	fmt.Println("API will be available at http://" + config.Host + ":" + config.Port + "/api/images")
	r := mux.NewRouter()
	r.HandleFunc("/api/images", getImages).Methods("GET")
	r.HandleFunc("/api/images/{id}", getImage).Methods("GET")
	r.HandleFunc("/api/images", createImages).Methods("POST")
	log.Fatal(http.ListenAndServe(config.Host+":"+config.Port, r))
}
