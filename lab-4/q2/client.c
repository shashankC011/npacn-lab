/*
    PROGRAM: TCP DNS Client

    Client sends domain names to server.
    Server responds with IP address.
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 1024

int main(){

    int sock;
    struct sockaddr_in serv_addr;

    char buffer[MAX];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    while(1){

        printf("\nEnter domain name (type 'exit' to stop): ");
        scanf("%s",buffer);

        write(sock,buffer,sizeof(buffer));

        if(strcmp(buffer,"exit")==0)
            break;

        read(sock,buffer,sizeof(buffer));

        printf("\nSERVER RESPONSE:\n%s\n",buffer);
    }

    close(sock);
}
