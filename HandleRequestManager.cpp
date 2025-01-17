#include"HandleRequestManager.h"
#include"httpserver.h"

bool HandleRequestManager::parse_request(char* request, std::string& method, std::string& url)
  {
    if (request == nullptr || strlen(request) == 0) {
        return false;
    }
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

 void HandleRequestManager::handle_json_get(std::string method, std::string url, std::string& resp)
{
        std::unordered_map<std::string, std::string> data = {
            {"message", "Hello, world!"},
            {"status", "200 OK"}
        };

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(Serializer::serializeToJson(data).length()) + "\r\n\r\n" +
               Serializer::serializeToJson(data);

        std::cout<<"json response:\n";
        for (const auto& [key, value] : data) {
        std::cout << "{" <<key << ": " << value << "}\n";
        }
        std::cout<<"\n";   
}

    void HandleRequestManager::handle_xml_get(std::string method, std::string url, std::string& resp) 
    {
        std::unordered_map<std::string, std::string> data = {
            {"message", "Hello, world!"},
            {"status", "200 OK"}
        };

        std::string xmlPayload = Serializer::serializeToXml(data);  
        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/xml\r\n"
               "Content-Length: " + std::to_string(Serializer::serializeToXml(data).length()) + "\r\n\r\n" +
               Serializer::serializeToXml(data);
}

void HandleRequestManager::handle_post(std::string method, std::string url, std::string& resp) {
    if (url.find("xml") != std::string::npos) {
        std::unordered_map<std::string, std::string> responseData = {
            {"status", "success"},
            {"message", "Data received successfully"},
            {"timestamp", std::to_string(time(nullptr))}
        };

        std::string xmlPayload = Serializer::serializeToXml(responseData);
        resp = "HTTP/1.1 201 Created\r\n"
               "Content-Type: application/xml\r\n"
               "Content-Length: " + std::to_string(xmlPayload.length()) + "\r\n\r\n" +
               xmlPayload;
    } else {
        std::unordered_map<std::string, std::string> responseData = {
            {"status", "success"},
            {"message", "Data received successfully"},
            {"timestamp", std::to_string(time(nullptr))}
        };

        resp = "HTTP/1.1 201 Created\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(Serializer::serializeToJson(responseData).length()) + "\r\n\r\n" +
               Serializer::serializeToJson(responseData);
    }
}

void HandleRequestManager::handle_put(std::string method, std::string url, std::string& resp) {
    if (url.find("xml") != std::string::npos) {
        std::unordered_map<std::string, std::string> data = {
            {"message", "Resource updated successfully"},
            {"status", "200 OK"},
            {"resource", url}
        };

        std::string xmlPayload = Serializer::serializeToXml(data);
        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/xml\r\n"
               "Content-Length: " + std::to_string(xmlPayload.length()) + "\r\n"
               "Connection: close\r\n\r\n" + xmlPayload;

        std::cout << "PUT XML response generated for: " << url << "\n";
    } else {
        std::unordered_map<std::string, std::string> data = {
            {"message", "Resource updated successfully"},
            {"status", "200 OK"},
            {"resource", url}
        };

        std::string jsonPayload = Serializer::serializeToJson(data);
        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(jsonPayload.size()) + "\r\n"
               "Connection: close\r\n\r\n" + jsonPayload;

        std::cout << "PUT response generated for: " << url << "\n";
    }
}
void HandleRequestManager::handle_delete(httpserver* server, std::string method, std::string url, std::string& resp) {
    std::unordered_map<std::string, std::string> data = {
        {"message", "Resource deleted successfully"},
        {"status", "200 OK"},
        {"resource", url}
    };

    std::cout << "Processing DELETE for URL: " << url << "\n";

    if (url.find("xml") != std::string::npos) {
        std::string xmlPayload = Serializer::serializeToXml(data);
        std::cout << "Generated XML Payload: " << xmlPayload << "\n";
        std::cout << "XML Payload Length: " << xmlPayload.length() << "\n";

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/xml\r\n"
               "Content-Length: " + std::to_string(xmlPayload.length()) + "\r\n"
               "Connection: close\r\n\r\n" + xmlPayload;
        std::cout << "DELETE XML response generated.\n";
    } else {
        std::string jsonPayload = Serializer::serializeToJson(data);
        std::cout << "Generated JSON Payload: " << jsonPayload << "\n";
        std::cout << "JSON Payload Length: " << jsonPayload.size() << "\n";

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(jsonPayload.size()) + "\r\n"
               "Connection: close\r\n\r\n" + jsonPayload;
        std::cout << "DELETE JSON response generated.\n";
    }
    bool deleted = server->delete_route(method, url);
    if (deleted) {
        std::cout << "Route successfully deleted.\n";
    } else {
        std::cout << "Route not found. Nothing to delete.\n";
    }
    server->print_routes();
}


void HandleRequestManager::handle_head(std::string method, std::string url, std::string &resp) {
    if (url.find("xml") != std::string::npos) {
        std::unordered_map<std::string, std::string> data = {
            {"status", "200 OK"}
        };

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/xml\r\n"
               "Content-Length: " + std::to_string(Serializer::serializeToXml(data).length()) + "\r\n\r\n";
    } else {
        std::unordered_map<std::string, std::string> data = {
            {"status", "200 OK"}
        };

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(Serializer::serializeToJson(data).length()) + "\r\n\r\n";
    }
}