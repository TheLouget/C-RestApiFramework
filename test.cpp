
#include <stdlib.h>
#include "httpserver.h"
#include "Serializer.h"
#include "HandleRequestManager.h"

int main() {
   
    httpserver server(8080, "127.0.0.1");

    server.add_route("GET", "/json", [](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_json_response(method, url, resp);
    });

    server.add_route("GET", "/xml", [](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_xml_response(method, url, resp);
    });

    server.add_route("POST", "/submit", [](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_post(method, url, resp);
    });

    server.run();

}
