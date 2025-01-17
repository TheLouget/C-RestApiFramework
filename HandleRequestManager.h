#ifndef HANDLE_REQUEST_MANAGER_H
#define HANDLE_REQUEST_MANAGER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include "Serializer.h"
class httpserver;

class HandleRequestManager {
public:
    bool parse_request(char* request, std::string& method, std::string& url);
    //Pentru a ignora request-urile inutile din firefox
    bool is_favicon(const std::string& url) {
        return url == "/favicon.ico";
    }
    void handle_favicon(const std::string& url) {
        if (url == "/favicon.ico") {
            std::cout << "Ignoring /favicon.ico request\n";
            return;
        }
    }
    void handle_json_get(std::string method, std::string url, std::string& resp);
    void handle_xml_get(std::string method, std::string url, std::string& resp);
    void handle_post(std::string method, std::string url, std::string& resp);
    void handle_head(std::string method, std::string url, std::string &resp);
    void handle_put(std::string method, std::string url, std::string& resp);
    void handle_delete(httpserver* server, std::string method, std::string url, std::string& resp);
};


#endif // HANDLE_REQUEST_MANAGER_H