#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include "Serializer.h"

class HandleRequestManager {
public:
    bool parse_request(char* request, std::string& method, std::string& url);
    bool is_favicon(const std::string& url) {
        return url == "/favicon.ico";
    }
    void handle_favicon(const std::string& url) {
        if (url == "/favicon.ico") {
            std::cout << "Ignoring /favicon.ico request\n";
            return;
        }
    }
    void handle_json_response(std::string method, std::string url, std::string& resp);
    void handle_xml_response(std::string method, std::string url, std::string& resp);
    void handle_post(std::string method, std::string url, std::string& resp);
    void handle_head(std::string method, std::string url, std::string& resp);
    void handle_put(std::string method, std::string url,  std::string& resp);
    void handle_delete(std::string method, std::string url,  std::string& resp);
};
