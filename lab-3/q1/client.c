#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>  //has close()


#define MAXSIZE 1024

int main(){
    struct sockaddr_in serveraddr,clientaddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    memset(&clientaddr,0,sizeof(clientaddr));

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        perror("Error creating socket.");
        return 1;
    }

    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(8080);
    
    if(connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) == -1){
        perror("Error connecting to server");
        close(sockfd);
        return 1;
    }
    printf("Connected to server on port: %d \n",ntohs(serveraddr.sin_port));
    socklen_t client_len = sizeof(clientaddr);

    getsockname(sockfd,(struct sockaddr*)&clientaddr,&client_len);
    printf("Connected from client using port: %d \n",ntohs(clientaddr.sin_port));

    char buf[MAXSIZE];
    while(1){
        memset(&buf,0,MAXSIZE);
    }
    close(sockfd);
}
