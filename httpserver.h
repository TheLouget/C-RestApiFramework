#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "connection.h"
#include "ThreadManager.h"
#include <unordered_map>
#include <string>
#include <functional>
class httpserver
{
    public:
    httpserver(unsigned short port,const char* ipaddr);
    ~httpserver();
    void add_route(std::string method,std::string path,std::function<void(std::string,std::string,std::string&)>handler);
    bool delete_route(std::string method, std::string path);
    void print_routes();
    void run();
    void route(std::string method,std::string path,std::string&route);
    private:
    void handlerequest(char*request,int i);
    Connection conexiune;
    ThreadManager threadManager;
    std::unordered_map<std::string,std::function<void(std::string,std::string,std::string&)>>routes;
    pthread_mutex_t routesMutex;
    struct sockaddr_in clientaddr[100];
    int client_sock[100];
};

#endif //HTTPSERVER_H