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
    void run();
    private:
    void handlerequest(char*request,int i);
    void route(std::string method,std::string path,std::string&route);
    Connection conexiune;
    ThreadManager threadManager;
    std::unordered_map<std::string,std::function<void(std::string,std::string,std::string&)>>routes;
    pthread_mutex_t routesMutex;
    struct sockaddr_in clientaddr[100];
    int client_sock[100];
};