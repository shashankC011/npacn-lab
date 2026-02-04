/*
    PROGRAM: UDP Server to Display Client Details

    DESCRIPTION:
    This program demonstrates connectionless communication using UDP sockets.
    The server receives a message from the client and displays the client's
    socket address, IP address, and port number on the server screen.

    Steps:
    1. Create UDP socket.
    2. Bind to port.
    3. Receive message using recvfrom().
    4. Extract client details from sockaddr_in structure.
    5. Display IP and port.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MAX 1024

int main() {

    int sockfd;
    char buffer[MAX];

    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    printf("UDP Server waiting for message...\n");

    addr_size = sizeof(clientAddr);

    // Receive message
    recvfrom(sockfd, buffer, MAX, 0,
             (struct sockaddr*)&clientAddr, &addr_size);

    printf("\nMessage received from client.\n");

    printf("Client IP Address : %s\n", inet_ntoa(clientAddr.sin_addr));
    printf("Client Port Number: %d\n", ntohs(clientAddr.sin_port));
    printf("Client Socket Address captured successfully.\n");

    close(sockfd);

    return 0;
}
