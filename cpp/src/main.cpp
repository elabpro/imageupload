/*
   Mathieu Stefani, 07 février 2016

   Example of a REST endpoint with routing
 * https://raw.githubusercontent.com/oktal/pistache/master/examples/rest_server.cc
 */

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <libfastjson/json.h>
#include <MPFDParser-1/Parser.h>
#include <curl/curl.h>
#include "DB.h"
#include "base64.h"
#include "Config.h"

#define DEBUG_MODE

using namespace std;
using namespace Pistache;
using namespace boost::filesystem;

void printCookies(const Http::Request& req) {
    auto cookies = req.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto& c : cookies) {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
}

namespace Generic {

    void handleReady(const Rest::Request&, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, "1");
    }

}

class StatsEndpoint {
public:

    StatsEndpoint(Address addr)
    : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {
    }

    void init(size_t thr = 2, bool mode = true, Config* c = new Config()) {
        auto opts = Http::Endpoint::options()
                .threads(thr)
                .flags(Tcp::Options::InstallSignalHandler)
                .maxPayload(maxPostSize);
        httpEndpoint->init(opts);
        setupRoutes();
        config = c;
        db = new DB(config->redisHost);
        if (mode) {
            check();
        }
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

    void shutdown() {
        httpEndpoint->shutdown();
    }

private:

    // Limit 10 Mb
    int maxPostSize = 10 * 1024 * 1024;
    Config* config;
    DB* db;

    void check() {
        int maxId = 0;
        if (access(config->imagesDir.c_str(), F_OK) == -1) {
            std::cout << "FATAL ERROR: Can't find " << config->imagesDir << endl;
            // exit(1);
        }
        if (access(config->imagesThumbDir.c_str(), F_OK) == -1) {
            std::cout << "FATAL ERROR: Can't find " << config->imagesThumbDir << endl;
            // exit(1);
        }
        for (auto & p : directory_iterator(config->imagesDir)) {
            std::size_t found = p.path().string().find(".lock");
            if (found != std::string::npos) {
                string filename = p.path().string().substr(0, found);
                cout << "Deleting lock for " << filename << endl;
                std::remove(filename.c_str());
                deleteLockForFile(filename);
            } else {
                int id = atoi(p.path().string().substr(config->imagesDir.length() + 1).c_str());
                if (id > maxId) {
                    maxId = id;
                }
            }
        }
                for (auto & p : directory_iterator(config->imagesThumbDir)) {
                    std::size_t found = p.path().string().find(".lock");
                    if (found != std::string::npos) {
                        string filename = p.path().string().substr(0, found);
                        int fileId = atoi(filename.substr(config->imagesThumbDir.length() + 1).c_str());
                        cout << "Recreating thumb for " << filename << endl;
                        createThumb(fileId);
                    } 
                }
        cout << "Setting counter to " << maxId << endl;
        db->setCounter(maxId);
    }

    /**
     * Setting routes for API
     */
    void setupRoutes() {
        using namespace Rest;

        Routes::Post(router, "/api/images", Routes::bind(&StatsEndpoint::doPost, this));
        Routes::Get(router, "/api/images", Routes::bind(&StatsEndpoint::doGetImages, this));
        Routes::Get(router, "/api/images/:id", Routes::bind(&StatsEndpoint::doGetImage, this));
    }

    /**
     * Method POST
     * Upload images in PNG. Body:
     * - multipart/encoded files
     * - JSON {{content=#base64_image_content#|url=#url#}}
     * - url=#url#
     *
     * @usage http://localhost/api/images
     *
     * @param req
     * @param response
     */
    void doPost(const Rest::Request& req, Http::ResponseWriter response) {
        fjson_object *images = fjson_object_new_array();
        std::string body = "";
        std::cout << "doPost" << endl;
        try {
            MPFD::Parser* POSTParser = new MPFD::Parser();
            POSTParser->SetTempDirForFileUpload("/tmp");
            POSTParser->SetMaxCollectedDataLength(maxPostSize);
            string body = req.body();
            string head = body.substr(0, 2);
#ifdef DEBUG_MODE
            cout << "Head:" << head << endl;
#endif
            if (head.compare("[{") != 0) {
                auto content_type = req.headers().tryGet<Pistache::Http::Header::ContentType>();
                POSTParser->SetContentType(content_type->mime().toString());
                POSTParser->AcceptSomeData(body.c_str(), body.length());
                // Now see what we have:
                std::map<std::string, MPFD::Field *> fields = POSTParser->GetFieldsMap();
#ifdef DEBUG_MODE
                std::cout << "Have " << fields.size() << " fields\n\r";
#endif
                std::map<std::string, MPFD::Field *>::iterator it;
                for (it = fields.begin(); it != fields.end(); it++) {
                    if (fields[it->first]->GetType() == MPFD::Field::TextType) {
                        cout << it->first << endl;
                        if (it->first.compare("url") == 0) {
                            string filename = fields[it->first]->GetTextTypeContent();
                            int fileId = downloadFile(filename);
                            fjson_object_array_add(images, fjson_object_new_string(std::to_string(fileId).c_str()));
                        }
                    } else {
#ifdef DEBUG_MODE
                        std::cout << "Got file field: [" << it->first << "] Filename:[" << fields[it->first]->GetFileName() << "] \n";
#endif
                        int fileId = createImageFromFile(fields[it->first]->GetTempFileName());
                        fjson_object_array_add(images, fjson_object_new_string(std::to_string(fileId).c_str()));
                    }
                }
            } else {
#ifdef DEBUG_MODE
                cout << "It must be JSON" << endl;
#endif
                fjson_object *jsonObj;
                jsonObj = fjson_tokener_parse(body.c_str());
                for (int i = 0; i < fjson_object_array_length(jsonObj); i++) {
                    fjson_object *obj = fjson_object_array_get_idx(jsonObj, i);
                    struct fjson_object_iterator it = fjson_object_iter_begin(obj);
                    struct fjson_object_iterator itEnd = fjson_object_iter_end(obj);
                    while (!fjson_object_iter_equal(&it, &itEnd)) {
                        string tagName = fjson_object_iter_peek_name(&it);
                        string tagValue = fjson_object_to_json_string(fjson_object_iter_peek_value(&it));
                        if (tagName.compare("url") == 0) {
                            int fileId = downloadFile(tagValue);
                            fjson_object_array_add(images, fjson_object_new_string(std::to_string(fileId).c_str()));
                        }
                        if (tagName.compare("content") == 0) {
                            int fileId = createImageFromContent(tagValue);
                            fjson_object_array_add(images, fjson_object_new_string(std::to_string(fileId).c_str()));
                        }
                        fjson_object_iter_next(&it);
                    }
                }
            }
        } catch (MPFD::Exception ex) {
            cout << "Exception:" << ex.GetError() << endl;
        }
        if (images != NULL) {
            body = fjson_object_to_json_string(images);
        }
        response.send(Http::Code::Ok, body);
    }

    /**
     * Method GET Getting array of images (JSON)
     * On return sends JSON array of filenames
     *
     * @usage http://localhost:8080/api/images
     *
     * @param request
     * @param response
     */
    void doGetImages(const Rest::Request& request, Http::ResponseWriter response) {
        fjson_object *images = fjson_object_new_array();
        std::string body = "";
        for (auto & p : directory_iterator(config->imagesDir)) {
            fjson_object_array_add(images, fjson_object_new_string(p.path().string().substr(config->imagesDir.length() + 1).c_str()));
        }
        if (images != NULL) {
            body = fjson_object_to_json_string(images);
        }
        response.send(Http::Code::Ok, body);
    }

    /**
     * Method GET Getting an image
     * On return sends binary image
     *
     * @usage http://localhost:8080/api/images/{id}
     *
     * @param request
     * @param response
     */
    void doGetImage(const Rest::Request& request, Http::ResponseWriter response) {
        auto id = request.param(":id").as<int>();
        auto stream = response.stream(Http::Code::Ok);
        std::ifstream fin(config->imagesDir + "/" + std::to_string(id), ios::binary);
        response.headers().add<Http::Header::ContentType>(MIME(Image, Png));
        char* binary_data = new char[1024];
        size_t chunk_size = sizeof (binary_data);
        size_t readed;
        int length = 0;
        while (!fin.eof()) {
            fin.read(binary_data, chunk_size);
            readed = fin.gcount();
            length += readed;
            stream.write(binary_data, readed);
            stream.flush();
        }
        response.headers().add<Pistache::Http::Header::ContentLength>(length);
        delete binary_data;
        stream.ends();
    }

    void writeToFile(const string & filename, const string & data) {
        std::ofstream out;
        out.open(filename.c_str());
        out << data;
        out.close();
    }

    /**
     * Create file from uploaded file
     * @param tmpFilename
     * @return 
     */
    int createImageFromFile(const string & tmpFilename) {
        int fileId = db->getCounter();
        string filename = getFilename(fileId);
        createLockForFile(filename);
        try {
            if (access(filename.c_str(), F_OK) != -1) {
                std::remove(filename.c_str());
            }
            boost::filesystem::copy(tmpFilename, filename);
            std::remove(tmpFilename.c_str());
        } catch (boost::filesystem::filesystem_error& e) {
            cout << e.what() << '\n';
        }
        deleteLockForFile(filename);
        createThumb(fileId);
        return fileId;
    }

    /**
     * Create file from base64 encoded content
     * 
     * @param in
     * @return 
     */
    int createImageFromContent(const string & in) {
        base64* b64 = new base64();
        int l = in.length();
        string content = in.substr(1, l - 2);
        config->replaceAll(content, "\\/", "/");
#ifdef DEBUG_MODE
        cout << "Trying to save from base64" << endl;
#endif
        int fileId = db->getCounter();
        string filename = getFilename(fileId);
        createLockForFile(filename);
        string out = b64->base64_decode(content);
        writeToFile(filename, out);
        deleteLockForFile(filename);
        createThumb(fileId);
        return fileId;
    }

    /**
     * Create a lock file 
     * 
     * @param filename
     * @return 
     */
    int createLockForFile(const string & filename) {
        std::ofstream outfile(filename + ".lock");
        outfile << "1" << std::endl;
        outfile.close();
        return 1;
    }

    /**
     * Delete a lock file 
     * 
     * @param filename
     * @return 
     */
    int deleteLockForFile(const string & filename) {
        string lockfilename = filename + ".lock";
        std::remove(lockfilename.c_str());
        return 1;
    }

    void createThumb(int fileId) {
        string imageFilename = getFilename(fileId);
        string filename = getThumbFilename(fileId);
        createLockForFile(filename);
        string cmd = "convert " + imageFilename + " -resize 100x100! " + filename;
        exec(cmd.c_str());
        deleteLockForFile(filename);
    }

    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose) > pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    /**
     * Download and save image to a file
     * 
     * @param urlName
     * @return 
     */
    int downloadFile(const string & urlName) {
        int fileId = db->getCounter();
        string filename = getFilename(fileId);
        createLockForFile(filename);
        string url = urlName;
        config->replaceAll(url, "\\/", "/");
        config->replaceAll(url, "\"", "");
        cout << "Trying to download file " << url << " into " << filename << endl;
        CURL *curl;
        FILE *fp;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            fp = fopen(filename.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            /* always cleanup */
            curl_easy_cleanup(curl);
            fclose(fp);
        }
        deleteLockForFile(filename);
        createThumb(fileId);
        return fileId;
    }

    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
    }

    string getFilename(int fileId) {
        string filename = config->imagesDir + "/" + std::to_string(fileId);
#ifdef DEBUG_MODE
        cout << "Filename:" << filename << endl;
#endif
        return filename;
    }

    string getThumbFilename(int fileId) {
        string filename = config->imagesThumbDir + "/" + std::to_string(fileId);
#ifdef DEBUG_MODE
        cout << "Filename:" << filename << endl;
#endif
        return filename;
    }


    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

Config* readConfig() {
    Config* c = new Config();
    string configFilename = "config.json";
    if (access(configFilename.c_str(), F_OK) != -1) {
        std::ifstream fin(configFilename.c_str());
        std::stringstream body;
        body << fin.rdbuf();
        fin.close();
        string bodyString = body.str();
        fjson_object *obj = fjson_tokener_parse(bodyString.c_str());
        struct fjson_object_iterator it = fjson_object_iter_begin(obj);
        struct fjson_object_iterator itEnd = fjson_object_iter_end(obj);
        while (!fjson_object_iter_equal(&it, &itEnd)) {
            string tagName = fjson_object_iter_peek_name(&it);
            string tagValue = fjson_object_to_json_string(fjson_object_iter_peek_value(&it));
            c->replaceAll(tagValue, "\\/", "/");
            c->replaceAll(tagValue, "\"", "");
            if (tagName.compare("redisHost") == 0) {
                c->redisHost = tagValue;
            }
            if (tagName.compare("host") == 0) {
                c->listenHost = tagValue;
            }
            if (tagName.compare("port") == 0) {
                c->listenPort = atoi(tagValue.c_str());
            }
            if (tagName.compare("imagesDir") == 0 && access(tagValue.c_str(), F_OK) != -1) {
                c->imagesDir = tagValue;
            }
            if (tagName.compare("imagesThumbDir") == 0 && access(tagValue.c_str(), F_OK) != -1) {
                c->imagesThumbDir = tagValue;
            }
            fjson_object_iter_next(&it);
        }
    }
    return c;
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int thr = 2;

    Config* c = readConfig();
    cout << "C++ project v.0.8. Image API is ready" << endl;
    cout << "URL http://localhost:" << c->listenPort << "/api/images" << endl;
    cout << "Source code from https://github.com/elabpro/imageupload" << endl;
    c->print();
    switch (pid = fork()) {
        case -1:
            perror("fork"); /* произошла ошибка */
            exit(1); /*выход из родительского процесса*/
        case 0:
        {
            Port port(8000);
            Address addr(Ipv4::any(), port);
            StatsEndpoint stats(addr);
            stats.init(thr, false, c);
            stats.start();
            stats.shutdown();
            break;
        }
        default:
        {
            Port port(c->listenPort);
            Address addr(Ipv4::any(), port);
            StatsEndpoint statsDef(addr);
            statsDef.init(thr, true, c);
            statsDef.start();
            statsDef.shutdown();
        }
    }
}
