#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
class Connection
{
    public:
        Connection(unsigned short port,const char* ipaddr);
        void run();
        void acceptconnection();
        void receive(char * client_message,int nrofbytes);
        void sendresponse(char * response);
    private:
    int serversock;
    int clientsock;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
};