#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#define MAXSIZE 90

int main()
{
    //create buffer
	char buff[MAXSIZE];

	int sockfd,clientSocketFd;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	
    //socket creation
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
	    perror("\nSocket creation error");
        return 1;
	}

    //zero initialize serveraddr
    memset(&serveraddr, 0, sizeof(serveraddr));

    //server address config
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=INADDR_ANY;

    //bind socket to server
	if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) == -1){
		perror("Binding error");
		close(sockfd);
        return 1;
    }

    //listen on server for 1 connection
	if(listen(sockfd,1) == -1){
        perror("Error listening for connections.");
	    close(sockfd);
        return 1;
    }

	actuallen=sizeof(clientaddr);
	clientSocketFd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
	if(clientSocketFd==-1)
	{
		close(sockfd);
        return 1;
	}
	
	while(1)
	{
		//clear buffer
		memset(buff, '\0', sizeof(buff));
        
        //wait and receive bytes
		recedbytes=recv(clientSocketFd,buff,sizeof(buff),0);
		if(recedbytes <= 0){
            perror("Did not receive any bytes.");
			close(sockfd);
			close(clientSocketFd);
            break;
		}

        buff[recedbytes] = '\0';

        if(strcmp(buff,"stop") == 0){
            printf("closing server.");
            break;
        }

		printf("received: %s \n",buff);

        //clear buffer
		memset(buff, '\0', sizeof(buff));

        //get message from stdin
        fgets(buff,MAXSIZE,stdin);
       
        //remove \n
        buff[strcspn(buff,"\n")] = '\0';

        //send the message
		sentbytes=send(clientSocketFd,buff,strlen(buff) + 1,0);
		if(sentbytes==-1)
		{
            perror("Error sending bytes.");
			close(sockfd);
			close(clientSocketFd);
            break;
		}

        if(strcmp(buff,"stop") == 0){
            perror("Closing server.");
            break;
        }
	}
	close(clientSocketFd);
	close(sockfd);
}