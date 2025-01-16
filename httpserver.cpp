#include "httpserver.h"
#include "HandleRequestManager.h"
#include <iostream>
#define REQSIZE 1024
httpserver::httpserver(unsigned short port, const char* ipaddr) : conexiune(port, ipaddr){
    conexiune.run();
}

void httpserver::add_route(std::string method, std::string path, std::function<void(std::string, std::string,std::string&)> handler)
{
    if(this->routes.find(method+" "+path)!=this->routes.end())
    {
        std::cout<<"Ruta deja existenta,nu s-a putut face rutarea pentru "<<method<<" "<<path;
        return;
    }
    else
    {
        this->routes.emplace(method+" "+path,handler);
    }
}
void httpserver::run() {
    while (true) {
        conexiune.acceptconnection();
            char buffer[REQSIZE];
            conexiune.receive(buffer, REQSIZE);

            threadManager.addTask([this,buffer]{
                this->handlerequest((char*)buffer);
            });
        }
    }

void httpserver::handlerequest(char* request) {
    std::string method, url;
    HandleRequestManager requestManager;
    if (requestManager.parse_request(request, method, url)) {
        std::string response;
        response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\n";
        this->route(method,url,response);
        this->conexiune.sendresponse((char*)response.data());
        return;
    }

    if (requestManager.is_favicon(url)) {
        requestManager.handle_favicon(url);
        return;
    }

    std::string response;
    if(method=="GET")
    {
        if (!strstr(url.c_str(),"/json")) {
        requestManager.handle_json_response(method, url, response);
        }
        else if (!strstr(url.c_str(),"/xml")) {
            requestManager.handle_xml_response(method, url, response);
        }
    }
    else if (method=="POST")
    {
        requestManager.handle_post(method, url, response);
    }
    else if(method=="HEAD")
    {
        requestManager.handle_head(method, url, response);
    }
    else if (method == "PUT") {
        requestManager.handle_put(method, url, response);
    } else if (method == "DELETE") {
       requestManager.handle_delete(method, url, response);
    }
    else {
        response ="HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 15\r\n\r\nRoute not found\n\r";
    }
    this->route(method,url,response);
    this->conexiune.sendresponse((char*)response.data());
   
}




void httpserver::route(std::string method, std::string path,std::string &response)
{
    auto it=this->routes.find(method+" "+path);
    if(it!=this->routes.end())
    {
        it->second(method,path,response);
        return;
    }
}
