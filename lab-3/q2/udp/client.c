/*
    PROGRAM: Half Duplex Chat Client using UDP

    DESCRIPTION:
    The client sends a string to the server.
    The server generates permutations and sends them back.
    The client displays the received permutations.

    Half Duplex:
    Client sends -> waits -> receives.
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

    char buffer[100];
    char result[MAX];

    // STEP 1: Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1){

        printf("\nEnter string (type 'exit' to stop): ");
        scanf("%s",buffer);

        // Send string
        sendto(sockfd, buffer, sizeof(buffer), 0,
                (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        if(strcmp(buffer,"exit")==0)
            break;

        // Receive permutations
        recvfrom(sockfd, result, sizeof(result), 0, NULL, NULL);

        printf("\nPermutations from server:\n%s\n",result);
    }

    close(sockfd);
}
