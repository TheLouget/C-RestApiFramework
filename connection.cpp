#include "connection.h"
#include <stdlib.h>
Connection::Connection(unsigned short port, const char *ipaddr)
{
    this->serversock = socket(AF_INET, SOCK_STREAM, 0);
    if(this->serversock < 0){
        printf("Error while creating socket\n");
        exit(-1);
    }
    printf("Socket created successfully\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ipaddr);
    while(bind(this->serversock, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        sleep(2);
    }
    printf("Done with binding\n");
}

void Connection::run()
{ 
    char server_message[2000];
    memset(server_message, '\0', sizeof(server_message));
    while(listen(this->serversock, 20) < 0)
    {
        sleep(2);
    }
    printf("\nListening for incoming connections.....\n");
    return ;

}

int Connection::acceptconnection(struct sockaddr_in* client_addr)
{
    int client_size = sizeof(client_addr);
    int clientsock= accept(this->serversock, (struct sockaddr*)client_addr, (socklen_t*)&client_size);
 
    if (clientsock < 0){
        printf("Can't accept\n");
        return clientsock;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
    
    return clientsock;
}

void Connection::receive(char* client_message, int nrofbytes,int clientsock) {
    memset(client_message, '\0', nrofbytes);
    ssize_t bytes_received = recv(clientsock, client_message, nrofbytes, 0);
    if (bytes_received < 0) {
        printf("Couldn't receive\n");
        return;
    }
        printf("Msg from client: %s\n", client_message);
        printf("Bytes received: %ld\n", bytes_received);
}
void Connection::sendresponse(char* server_message, int client_sock) {
    printf("Response being sent to client:\n%s\n", server_message);

    int total_sent = 0;
    int message_length = strlen(server_message);
    printf("Message length: %d\n", message_length);

    while (total_sent < message_length) {
        int bytes_sent = send(client_sock, server_message + total_sent, message_length - total_sent, 0);
        if (bytes_sent < 0) {
            perror("Failed to send response");
            close(client_sock);
            return;
        }
        total_sent += bytes_sent;
        printf("Bytes sent: %d, Total sent: %d\n", bytes_sent, total_sent);
    }

    printf("Response fully sent.\n");
    close(client_sock);
    printf("Connection closed.\n");
}
