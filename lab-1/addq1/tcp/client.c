/*
    PROGRAM: TCP Client

    DESCRIPTION:
    This client connects to the TCP server.
    Once the connection is established, the server displays
    the client’s IP address and port number.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {

    int sock;
    struct sockaddr_in serv_addr;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Connected to TCP server.\n");

    close(sock);

    return 0;
}
