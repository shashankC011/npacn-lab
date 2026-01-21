#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>  //has close()

int main(){
    struct sockaddr_in serveraddr,clientaddr;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd == -1){
        perror("\nSOCKET CREATION ERROR");
        return 1;
    }
    memset(&serveraddr,0,sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8080);
    serveraddr.sin_addr.s_addr=INADDR_ANY;

    if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) == -1){
        perror("Binding error.");
        close(sockfd);
        return 1;
    }

    if(listen(sockfd,1) == -1){
        perror("Error listening for connections.");
        close(sockfd);
        return 1;
    }

    socklen_t client_len = sizeof(clientaddr);
    int clientsockfd = accept(sockfd,(struct sockaddr*)&clientaddr,&client_len);
    if (clientsockfd == -1){
        perror("Error accepting connection from client.");
        close(sockfd);
        return 1;
    }
    printf("\nSuccessfully connected to client with port: %d\n",ntohs(clientaddr.sin_port));
    close(sockfd);
    close(clientsockfd);
}