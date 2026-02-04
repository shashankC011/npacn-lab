/*
    PROGRAM: UDP Client

    DESCRIPTION:
    This client sends a message to the UDP server.
    The server then displays the client’s IP address and port number.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main() {

    int sockfd;
    char message[] = "Hello Server";

    struct sockaddr_in serverAddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send message
    sendto(sockfd, message, sizeof(message), 0,
           (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    printf("Message sent to UDP server.\n");

    close(sockfd);

    return 0;
}
