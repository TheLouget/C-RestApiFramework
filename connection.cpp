
#include "connection.h"
#include <stdlib.h>
Connection::Connection(unsigned short port, const char *ipaddr)
{
    // Create socket:
    this->serversock = socket(AF_INET, SOCK_STREAM, 0);
 
    if(this->serversock < 0){
        printf("Error while creating socket\n");
        exit(-1);
    }
    printf("Socket created successfully\n");
 
    // Set port and IP that we'll be listening for, any other IP_SRC or port will be dropped:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ipaddr);
 
    // Bind to the set port and IP:
    while(bind(this->serversock, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        sleep(2);
    }
    printf("Done with binding\n");
}

void Connection::run()
{ 
    char server_message[2000];
 
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    // Listen for clients:
    while(listen(this->serversock, 20) < 0)
    {
        sleep(2);
    }
    printf("\nListening for incoming connections.....\n");
    return ;

}

void Connection::acceptconnection()
{
    // Accept an incoming connection from one of the clients:
    int client_size = sizeof(client_addr);
    this->clientsock= accept(this->serversock, (struct sockaddr*)&client_addr, (socklen_t*)&client_size);
 
    if (this->clientsock < 0){
        printf("Can't accept\n");
        return ;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
 
}

void Connection::receive(char* client_message, int nrofbytes) {
    memset(client_message, '\0', nrofbytes);
    ssize_t bytes_received = recv(this->clientsock, client_message, nrofbytes, 0);
    if (bytes_received < 0) {
        printf("Couldn't receive\n");
        return;
    }
    printf("Msg from client: %s\n", client_message);  // Afișăm mesajul primit
}

void Connection::sendresponse(char * server_message)
{
    printf("%s",server_message);
    if (send(this->clientsock, server_message, strlen(server_message), 0) < 0){
        printf("Can't send\n");
        return ;
    }
    close(this->clientsock);
}
