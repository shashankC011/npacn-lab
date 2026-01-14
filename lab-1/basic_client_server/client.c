#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

#define MAXSIZE 50

int main()
{
	char buff[MAXSIZE];
	int sockfd,recedbytes,sentbytes;
	struct sockaddr_in serveraddr;

	//socket creation
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		perror("\nSocket Creation Error");
		return 1;
	}

	//server address setup
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	//connect to server
	int retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		perror("Connection error");
		return 1;
	}else{
		printf("Connection established.");
	}

	while(1)
	{
		//clear buffer
		memset(buff, 0, MAXSIZE);

		printf("Enter the text:\n");
		//use fgets over scanf, as scanf stops at whitespace
		if(fgets(buff,MAXSIZE,stdin) == NULL){
			break;
		}

		//line below is not needed as fgets adds null terminator 
		// buff[strlen(buff)] = '\0';

		//removes the newline character which can causes inconsistency with strcmp and with how output looks
		buff[strcspn(buff,"\n")] = '\0';

		//not needed as sizeof(char) is 1
		// int s = strlen(buff) * sizeof(char);

		//strlen(buff) + 1 (for null terminator)
		sentbytes=send(sockfd,buff,strlen(buff) + 1,0);

		//if sending bytes failed
		if(sentbytes==-1)
		{
			perror("Send failed");
			close(sockfd);
		}

		//CLIENT stop condition check
		if(strcmp(buff,"stop") == 0){
			break;
		}

		//clear buffer
		memset(buff, '\0', sizeof(buff));

		//recv DOES NOT ADD A NULL terminator(i.e. "\0" should be added manually)
		recedbytes=recv(sockfd,buff,sizeof(buff) -1,0);
		if(recedbytes <= 0){
			printf("server disconnected\n");
			break;
		}
		buff[recedbytes] = '\0';
		printf ("%s \n", buff);
		
		if(strcmp(buff,"stop") == 0){
			break;
		}
	}
	close(sockfd);
	return 0;
}