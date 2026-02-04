/*
    PROGRAM: TCP Server to Display Client Details

    DESCRIPTION:
    This program demonstrates connection-oriented communication using TCP sockets.
    The server waits for a client to connect. Once connected, it retrieves and
    displays the client's socket address, IP address, and port number on the
    server screen.

    Steps:
    1. Create a socket.
    2. Bind it to an IP and port.
    3. Listen for incoming connections.
    4. Accept the client.
    5. Extract and display client IP and port using inet_ntoa() and ntohs().
    6. Close sockets.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {

    int server_fd, new_socket;
    struct sockaddr_in address, client;
    socklen_t addrlen = sizeof(client);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen
    listen(server_fd, 3);

    printf("TCP Server waiting for connection...\n");

    // Accept client
    new_socket = accept(server_fd, (struct sockaddr *)&client, &addrlen);

    printf("\nClient Connected!\n");

    printf("Client IP Address : %s\n", inet_ntoa(client.sin_addr));
    printf("Client Port Number: %d\n", ntohs(client.sin_port));
    printf("Client Socket Address captured successfully.\n");

    close(new_socket);
    close(server_fd);

    return 0;
}
