/*
    PROGRAM: Full Duplex Chat Server using TCP

    DESCRIPTION:
    This program demonstrates a full duplex chat application between
    a server and a single client using connection-oriented TCP sockets.

    Full duplex means BOTH sides can send and receive messages at the same time.

    fork() is used to create two processes:
        Parent -> Sends messages
        Child  -> Receives messages

    The program also displays PID and PPID of parent and child processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[MAX];

    // STEP 1: Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // STEP 2: Bind socket
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // STEP 3: Listen for connection
    listen(server_fd, 3);

    printf("Server waiting for connection...\n");

    // STEP 4: Accept client
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    printf("Client connected!\n");

    // Create child process
    pid_t pid = fork();

    if(pid > 0){ 
        // PARENT PROCESS -> SEND MESSAGES

        printf("\nSERVER PARENT PROCESS\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());

        while(1){

            fgets(buffer, MAX, stdin);

            // Send message to client
            write(new_socket, buffer, strlen(buffer)+1);

            // Exit condition
            if(strncmp(buffer,"exit",4)==0)
                break;
        }
    }
    else{
        // CHILD PROCESS -> RECEIVE MESSAGES

        printf("\nSERVER CHILD PROCESS\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());

        while(1){

            read(new_socket, buffer, sizeof(buffer));

            printf("\nClient: %s",buffer);

            if(strncmp(buffer,"exit",4)==0)
                break;
        }
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
