#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
class Connection {
public:
    Connection(unsigned short port, const char* ipaddr);
    void run();
    int acceptconnection(struct sockaddr_in*client_addr);
    void receive(char* client_message, int nrofbytes,int client_sock);
    void sendresponse(char* response,int client_sock);
private:
    int serversock;
    struct sockaddr_in server_addr;
};