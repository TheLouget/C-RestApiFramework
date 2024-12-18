
#include <stdlib.h>
#include "httpserver.h"
#include "Serializer.h"
#include "HandleRequestManager.cpp"

int main() {
   
    httpserver server(8080, "127.0.0.1");

    server.add_route("GET", "/hello/json", [](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_json_response(method, url, resp);
    });

    server.add_route("GET", "/hello/xml", [](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_xml_response(method, url, resp);
    });

    server.add_route("POST", "/submit", [](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_post(method, url, resp);
    });

    server.add_route("HEAD","/hello",[](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_head(method, url, resp);
    });

        server.add_route("PUT","/hello/json",[](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_put(method, url, resp);
    });

        server.add_route("DELETE","/hello/json",[](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_delete(method, url, resp);
    });


    server.run();

}
