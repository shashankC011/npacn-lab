/*
    MULTI CLIENT PROGRAM
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9090

int main(){

    int sock;
    struct sockaddr_in serv_addr;

    char message[100];
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0){
        perror("Socket failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server.\n");

    printf("Enter message: ");
    scanf(" %[^\n]",message);

    write(sock,message,sizeof(message));

    // check termination
    read(sock,buffer,sizeof(buffer));

    if(strcmp(buffer,"terminate session")==0){
        printf("Server terminated session.\n");
        close(sock);
        exit(0);
    }

    close(sock);
}
