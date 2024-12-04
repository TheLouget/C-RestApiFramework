
#include <stdlib.h>
#include "httpserver.h"

void cacat(std::string method,std::string url,std::string& resp)
{
    resp="HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
}
int main()
{
    httpserver server(8080,"127.0.0.1");
    server.add_route("GET","/hello",cacat);
    server.run();
    return 0;
}