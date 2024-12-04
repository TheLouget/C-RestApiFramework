#include "httpserver.h"
#include <iostream>
#define REQSIZE 1024
httpserver::httpserver(unsigned short port, const char *ipaddr):conexiune(port,ipaddr)
{
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

void httpserver::run()
{
    char *buffer=new char[REQSIZE];
    while(1)
    {
    this->conexiune.acceptconnection();
    this->conexiune.receive(buffer,REQSIZE);
    this->handlerequest(buffer);
    }
    delete buffer;
}

void httpserver::handlerequest(char * request)
{
    int i=0;
    std::string method,url;
    char *token;
    token = strtok(request, " ");
    if (token != NULL) {
        method=token;
    }
    token = strtok(NULL, " ");
    if (token != NULL) {
        url=token;
    }
    std::string response;
    this->route(method,url,response);
    this->conexiune.sendresponse(response.data());
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
