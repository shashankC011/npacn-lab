/*
    PROGRAM: UDP File Operations Client

    DESCRIPTION:
    This client sends a filename to the server and performs operations
    like search, replace, and reorder based on the user's choice.

    UDP is connectionless — therefore no connect().
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 2000

int main(){

    int sockfd;
    struct sockaddr_in serverAddr;

    char buffer[MAX];
    char filename[100];
    int choice;

    // STEP 1: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    // Send filename
    printf("Enter filename: ");
    scanf("%s",filename);

    sendto(sockfd, filename, sizeof(filename), 0,
            (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

    if(strcmp(buffer,"File not present")==0){
        printf("Server: File not present\n");
        close(sockfd);
        return 0;
    }

    while(1){

        printf("\n1.Search\n2.Replace\n3.Reorder\n4.Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        sendto(sockfd, &choice, sizeof(choice), 0,
                (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        if(choice==4)
            break;

        if(choice==1){

            char word[100];

            printf("Enter string to search: ");
            scanf("%s",word);

            sendto(sockfd, word, sizeof(word), 0,
                    (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        }

        else if(choice==2){

            char str1[100],str2[100];

            printf("Enter string to replace: ");
            scanf("%s",str1);

            printf("Enter new string: ");
            scanf("%s",str2);

            sendto(sockfd, str1, sizeof(str1), 0,
                    (struct sockaddr*)&serverAddr, sizeof(serverAddr));

            sendto(sockfd, str2, sizeof(str2), 0,
                    (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        }

        recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

        printf("Server: %s\n",buffer);
    }

    close(sockfd);
}
