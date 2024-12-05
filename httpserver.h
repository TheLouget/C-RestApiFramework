#include "connection.h"
#include "ThreadManager.h"
#include <unordered_map>
#include <string>
#include <functional>
class httpserver
{
    public:
    httpserver(unsigned short port,const char* ipaddr);
    void add_route(std::string method,std::string path,std::function<void(std::string,std::string,std::string&)>handler);
    void run();
    private:
    void handlerequest(char* request);
    void route(std::string method,std::string path,std::string&route);
    Connection conexiune;
    ThreadManager threadManager;
    std::unordered_map<std::string,std::function<void(std::string,std::string,std::string&)>>routes;
};