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
#include "DB.h"

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

    void init(size_t thr = 2) {
        // 10 Mb
        auto opts = Http::Endpoint::options()
                .threads(thr)
                .flags(Tcp::Options::InstallSignalHandler)
                .maxPayload(maxPostSize);
        httpEndpoint->init(opts);
        setupRoutes();
        db = new DB();
        db->setCounter(0);
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

    void shutdown() {
        httpEndpoint->shutdown();
    }

private:

    std::string path = "images.full";
    int maxPostSize = 10 * 1024 * 1024;
    DB* db;

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
     * 
     * @param req
     * @param response
     */
    void doPost(const Rest::Request& req, Http::ResponseWriter response) {
        std::cout << "doPost" << endl;
        try {
            MPFD::Parser* POSTParser = new MPFD::Parser();
            POSTParser->SetTempDirForFileUpload("/tmp");
            POSTParser->SetMaxCollectedDataLength(maxPostSize);
            string body = req.body();
            string head = body.substr(0, 2);
            cout << "Head:" << head << endl;
            if (head.compare("[{") != 0) {
                auto content_type = req.headers().tryGet<Pistache::Http::Header::ContentType>();
                POSTParser->SetContentType(content_type->mime().toString());
                POSTParser->AcceptSomeData(body.c_str(), body.length());
                // Now see what we have:
                std::map<std::string, MPFD::Field *> fields = POSTParser->GetFieldsMap();

                std::cout << "Have " << fields.size() << " fields\n\r";

                std::map<std::string, MPFD::Field *>::iterator it;
                for (it = fields.begin(); it != fields.end(); it++) {
                    if (fields[it->first]->GetType() == MPFD::Field::TextType) {
                        cout << it->first << endl;
                        if (it->first.compare("url") == 0) {
                            string filename = fields[it->first]->GetTextTypeContent();
                            downloadFile(filename);
                        }
                    } else {
                        std::cout << "Got file field: [" << it->first << "] Filename:[" << fields[it->first]->GetFileName() << "] \n";
                        createImageFromFile(fields[it->first]->GetTempFileName());
                    }
                }
            } else {
                cout << "It must be JSON" << endl;
                fjson_object *jsonObj;
                jsonObj = fjson_tokener_parse(body.c_str());
                printf("my_array=\n");
                for (int i = 0; i < fjson_object_array_length(jsonObj); i++) {
                    fjson_object *obj = fjson_object_array_get_idx(jsonObj, i);
                    printf("my_object=\n");
                    struct fjson_object_iterator it = fjson_object_iter_begin(obj);
                    struct fjson_object_iterator itEnd = fjson_object_iter_end(obj);
                    while (!fjson_object_iter_equal(&it, &itEnd)) {
                        string tagName = fjson_object_iter_peek_name(&it);
                        string tagValue = fjson_object_to_json_string(fjson_object_iter_peek_value(&it));
                        if (tagName.compare("url") == 0) {
                            downloadFile(tagValue);
                        }
                        if (tagName.compare("content") == 0) {
                            cout << "Need to parse base64\n";
                        }
                        fjson_object_iter_next(&it);
                    }
                }
            }
        } catch (MPFD::Exception ex) {
            cout << "Exception:" << ex.GetError() << endl;
        }
        response.send(Http::Code::Ok);
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
        for (auto & p : directory_iterator(path)) {
            fjson_object_array_add(images, fjson_object_new_string(p.path().string().substr(path.length() + 1).c_str()));
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
        std::ifstream fin(path + "/" + std::to_string(id), ios::binary);
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

    int createImageFromFile(const string & tmpFilename) {
        string filename = getFilename();
        createLockForFile(filename);
        try {
            boost::filesystem::rename(tmpFilename, "/tmp/test.png");
        } catch (boost::filesystem::filesystem_error& e) {
            cout << e.what() << '\n';
        }
        deleteLockForFile(filename);
        return 1;
    }

    int createLockForFile(const string & filename) {
        std::ofstream outfile(filename + ".lock");
        outfile << "1" << std::endl;
        outfile.close();
        return 1;
    }

    int deleteLockForFile(const string & filename) {
        string lockfilename = filename + ".lock";
        std::ofstream outfile(lockfilename);
        std::remove(lockfilename.c_str());
        return 1;
    }

    void downloadFile(const string & filename) {
        string f = filename;
        replaceAll(f, "\\/", "/");
        cout << "Trying to download file " << f << endl;
    }

    void replaceAll(std::string& str, const std::string& from, const std::string& to) {
        if (from.empty())
            return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    string getFilename() {
        cout << "New id:" << db->getCounter() << endl;
        return "/tmp/test";
    }

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

int main(int argc, char *argv[]) {
    Port port(8080);

    int thr = 2;

    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    StatsEndpoint stats(addr);
    stats.init(thr);
    stats.start();
    stats.shutdown();
}
