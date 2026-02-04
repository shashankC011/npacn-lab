/*
    TCP Client for Student Structure Server
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MAX 1024

int main(){

    int sock, option;
    struct sockaddr_in serv_addr;
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    while(1){

        printf("\n1. Registration Number");
        printf("\n2. Student Name");
        printf("\n3. Subject Code");
        printf("\n4. Exit");

        printf("\nEnter option: ");
        scanf("%d",&option);

        write(sock,&option,sizeof(option));

        if(option==4) break;

        printf("Enter detail: ");
        scanf(" %[^\n]",buffer);

        write(sock,buffer,sizeof(buffer));

        read(sock,buffer,sizeof(buffer));

        printf("\nSERVER RESPONSE:\n%s\n",buffer);
    }

    close(sock);
}
