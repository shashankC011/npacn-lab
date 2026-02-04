/*
    PROGRAM: Full Duplex Chat Client using TCP

    DESCRIPTION:
    This client connects to the server and enables two-way communication.

    fork() is used:
        Parent -> Sends messages
        Child  -> Receives messages

    PID and PPID of both processes are displayed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main(){

    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAX];

    // STEP 1: Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // STEP 2: Connect to server
    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    printf("Connected to server.\n");

    pid_t pid = fork();

    if(pid > 0){
        // PARENT -> SEND

        printf("\nCLIENT PARENT PROCESS\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());

        while(1){

            fgets(buffer, MAX, stdin);

            write(sock, buffer, strlen(buffer)+1);

            if(strncmp(buffer,"exit",4)==0)
                break;
        }
    }
    else{
        // CHILD -> RECEIVE

        printf("\nCLIENT CHILD PROCESS\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());

        while(1){

            read(sock, buffer, sizeof(buffer));

            printf("\nServer: %s",buffer);

            if(strncmp(buffer,"exit",4)==0)
                break;
        }
    }

    close(sock);

    return 0;
}
