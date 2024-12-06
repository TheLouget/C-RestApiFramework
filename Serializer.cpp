#include "Serializer.h"

// void json_route(std::string method, std::string url, std::string& resp) {
//     std::unordered_map<std::string, std::string> data = {
//         {"message", "Hello, world!"},
//         {"status", "200 OK"}
//     };

//     resp = "HTTP/1.1 200 OK\r\n"
//            "Content-Type: application/json\r\n"
//            "Content-Length: " + std::to_string(Serializer::serializeToJson(data).length()) + "\r\n\r\n" +
//            Serializer::serializeToJson(data);
// }

// void xml_route(std::string method, std::string url, std::string& resp) {
//     std::unordered_map<std::string, std::string> data = {
//         {"message", "Hello, world!"},
//         {"status", "200 OK"}
//     };

//     resp = "HTTP/1.1 200 OK\r\n"
//            "Content-Type: application/xml\r\n"
//            "Content-Length: " + std::to_string(Serializer::serializeToXml(data).length()) + "\r\n\r\n" +
//            Serializer::serializeToXml(data);
//}


