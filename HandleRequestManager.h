#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include "Serializer.h"

class HandleRequestManager {
public:
    bool parse_request(char* request, std::string& method, std::string& url) {
        char* token;
        token = strtok(request, " ");
        if (token != NULL) {
            method = token; 
        } else {
            return false;
        }

        token = strtok(NULL, " ");
        if (token != NULL) {
            url = token;
        } else {
            return false;
        }

        return true;
    }


    bool is_favicon(const std::string& url) {
        return url == "/favicon.ico";
    }

    void handle_favicon(const std::string& url) {
        if (url == "/favicon.ico") {
            std::cout << "Ignoring /favicon.ico request\n";
            return;
        }
    }

 
    void handle_json_response(std::string method, std::string url, std::string& resp) {
        std::unordered_map<std::string, std::string> data = {
            {"message", "Hello, world!"},
            {"status", "200 OK"}
        };

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(Serializer::serializeToJson(data).length()) + "\r\n\r\n" +
               Serializer::serializeToJson(data);
    }

    void handle_xml_response(std::string method, std::string url, std::string& resp) {
        std::unordered_map<std::string, std::string> data = {
            {"message", "Hello, world!"},
            {"status", "200 OK"}
        };

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/xml\r\n"
               "Content-Length: " + std::to_string(Serializer::serializeToXml(data).length()) + "\r\n\r\n" +
               Serializer::serializeToXml(data);
    }


    void handle_post(std::string method, std::string url, std::string& resp) {
        std::string htmlContent = R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>POST Response</title>
            </head>
            <body>
                <h1>POST Received</h1>
                <p>Data received and processed ig.</p>
            </body>
            </html>
        )";

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n"
               "Connection: close\r\n\r\n" + htmlContent;
    }


};
