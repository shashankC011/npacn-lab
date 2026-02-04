/*
    UDP CLIENT PROGRAM
    -------------------
    Aim:
    To send a string to the UDP server and receive the result containing
    palindrome status, string length, and vowel count.

    Algorithm:
    1. Create a UDP socket.
    2. Enter a string from the user.
    3. Send the string to the server using sendto().
    4. Receive the processed result using recvfrom().
    5. Display the server response.
    6. Repeat until the user enters "Halt".
    7. Close the socket.

    Compile: gcc client_udp.c -o client
    Run: ./client
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 1024

int main(){

    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[MAX];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1){

        printf("\nEnter string (Halt to stop): ");
        fgets(buffer,MAX,stdin);
        buffer[strcspn(buffer,"\n")] = 0;

        sendto(sockfd,buffer,strlen(buffer)+1,0,
                (struct sockaddr*)&serverAddr,sizeof(serverAddr));

        if(strcmp(buffer,"Halt")==0)
            break;

        recvfrom(sockfd,buffer,MAX,0,NULL,NULL);

        printf("\nServer Response:\n%s\n",buffer);
    }

    close(sockfd);
}
