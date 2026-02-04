/*
    MULTIPLE CLIENT PROGRAM
    Sends two strings to iterative server.
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

    char str1[100], str2[100], buffer[200];

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

    printf("Enter first string: ");
    scanf("%s",str1);

    printf("Enter second string: ");
    scanf("%s",str2);

    // SEND STRINGS
    write(sock,str1,sizeof(str1));
    write(sock,str2,sizeof(str2));

    // RECEIVE RESULT
    read(sock,buffer,sizeof(buffer));

    printf("\nSERVER RESPONSE:\n%s\n",buffer);

    close(sock);
}
