#include"HandleRequestManager.h"

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

 void HandleRequestManager::handle_json_response(std::string method, std::string url, std::string& resp)
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

    void HandleRequestManager::handle_xml_response(std::string method, std::string url, std::string& resp) 
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

    void HandleRequestManager::handle_head(std::string method, std::string url, std::string &resp)
    {
        std::unordered_map<std::string, std::string> data = {
            {"status", "200 OK"}
        };

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(Serializer::serializeToJson(data).length()) + "\r\n\r\n";
    }

void HandleRequestManager::handle_put(std::string method, std::string url, std::string& resp) {
    // Exemplu de actualizare a unei resurse
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

void HandleRequestManager::handle_delete(std::string method, std::string url, std::string& resp) {
    // Exemplu de ștergere a unei resurse
    std::unordered_map<std::string, std::string> data = {
        {"message", "Resource deleted successfully"},
        {"status", "200 OK"},
        {"resource", url}
    };

    std::string jsonPayload = Serializer::serializeToJson(data);
    resp = "HTTP/1.1 200 OK\r\n"
           "Content-Type: application/json\r\n"
           "Content-Length: " + std::to_string(jsonPayload.size()) + "\r\n"
           "Connection: close\r\n\r\n" + jsonPayload;

    std::cout << "DELETE response generated for: " << url << "\n";
}
