/* 
   Mathieu Stefani, 07 février 2016
   
   Example of a REST endpoint with routing
 * https://raw.githubusercontent.com/oktal/pistache/master/examples/rest_server.cc
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <libfastjson/json.h>

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
        auto opts = Http::Endpoint::options()
                .threads(thr)
                .flags(Tcp::Options::InstallSignalHandler);
        httpEndpoint->init(opts);
        setupRoutes();
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

    void setupRoutes() {
        using namespace Rest;

        Routes::Post(router, "/api/images", Routes::bind(&StatsEndpoint::doPost, this));
        Routes::Get(router, "/api/images", Routes::bind(&StatsEndpoint::doGetImages, this));
        Routes::Get(router, "/api/images/:id", Routes::bind(&StatsEndpoint::doGetImage, this));
    }

    void doPost(const Rest::Request& req, Http::ResponseWriter response) {
        std::cout << "doPost" << req.param("url").as<std::string>() << endl;
        if (req.method() == Http::Method::Post) {

            using namespace Http;

            auto query = req.query();

            cout << req.headers().get("Content-type") << endl;

            //                if (req.method() == Http::Method::Post) {
            //                    writeToFile("test", req.body());
            //                    response.send(Http::Code::Ok, "ok");
            //                } else {
            //                    response.send(Http::Code::Ok, "No parameters");
            //                }

        }
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
        std::ifstream fin(path + "/" + std::to_string(id), ios::binary);
        ostringstream ostrm;
        ostrm << fin.rdbuf();
        string buf(ostrm.str());
        response.headers().add<Http::Header::ContentType>(MIME(Image, Png));
        response.headers().add<Pistache::Http::Header::ContentLength>(buf.length());
        response.send(Http::Code::Ok, buf);
    }

    //    void doRecordMetric(const Rest::Request& request, Http::ResponseWriter response) {
    //        auto name = request.param(":name").as<std::string>();
    //
    //        Guard guard(metricsLock);
    //        auto it = std::find_if(metrics.begin(), metrics.end(), [&](const Metric & metric) {
    //            return metric.name() == name;
    //        });
    //
    //        int val = 1;
    //        if (request.hasParam(":value")) {
    //            auto value = request.param(":value");
    //            val = value.as<int>();
    //        }
    //
    //        if (it == std::end(metrics)) {
    //            metrics.push_back(Metric(std::move(name), val));
    //            response.send(Http::Code::Created, std::to_string(val));
    //        } else {
    //            auto &metric = *it;
    //            metric.incr(val);
    //            response.send(Http::Code::Ok, std::to_string(metric.value()));
    //        }
    //
    //    }

    //    void doGetMetric(const Rest::Request& request, Http::ResponseWriter response) {
    //        auto name = request.param(":name").as<std::string>();
    //
    //        Guard guard(metricsLock);
    //        auto it = std::find_if(metrics.begin(), metrics.end(), [&](const Metric & metric) {
    //            return metric.name() == name;
    //        });
    //
    //        if (it == std::end(metrics)) {
    //            response.send(Http::Code::Not_Found, "Metric does not exist");
    //        } else {
    //            const auto& metric = *it;
    //            response.send(Http::Code::Ok, std::to_string(metric.value()));
    //        }
    //
    //    }

    //    void doAuth(const Rest::Request& request, Http::ResponseWriter response) {
    //        printCookies(request);
    //        response.cookies()
    //                .add(Http::Cookie("lang", "en-US"));
    //        response.send(Http::Code::Ok);
    //    }

    void writeToFile(const string & filename, const string & data) {
        std::ofstream out;
        out.open(filename.c_str());
        out << data;
        out.close();

    }

    typedef std::mutex Lock;
    typedef std::lock_guard<Lock> Guard;
    Lock metricsLock;

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
