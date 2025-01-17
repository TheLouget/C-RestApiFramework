#include <stdlib.h>
#include "httpserver.h"
#include "Serializer.h"
#include "HandleRequestManager.cpp"

int main() {

    httpserver server(8080, "127.0.0.1");
    HandleRequestManager manager;

    server.add_route("GET", "/resource", [](std::string method, std::string url, std::string& resp) {
        if (url.find("xml") != std::string::npos) {
            HandleRequestManager manager;
            manager.handle_xml_get(method, url, resp);
        } else {
            HandleRequestManager manager;
            manager.handle_json_get(method, url, resp);
        }
        std::cout << "Handled GET request for: " << url << "\n";
    });

   
    server.add_route("POST", "/resource", [](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_post(method, url, resp);
    });

    server.add_route("POST", "/resource/xml", [](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_post(method, url, resp);
    });


    server.add_route("HEAD","/resource",[](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_head(method, url, resp);
    });

    server.add_route("HEAD","/resource/xml",[](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_head(method, url, resp);
    });

        server.add_route("PUT","/resource/json",[](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_put(method, url, resp);
    });

        server.add_route("PUT","/resource/xml",[](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_put(method, url, resp);
    });    

        server.add_route("DELETE","/resource",[&server](std::string method, std::string url, std::string& resp) {
        HandleRequestManager manager;
        manager.handle_delete(&server,method, url, resp);
    });


    std::cout << "Server is running at http://127.0.0.1:8080\n";
    server.run();

    return 0;


}
