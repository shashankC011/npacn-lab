/*
    PROGRAM: Half Duplex Chat Client using TCP

    DESCRIPTION:
    The client sends a string to the server.
    The server returns all permutations.
    The client prints them.

    HALF DUPLEX:
    Client sends -> waits -> receives.
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 5000

int main(){

    int sock;
    struct sockaddr_in serv_addr;

    char buffer[100];
    char result[MAX];

    // STEP 1: Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // STEP 2: Connect to server
    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    while(1){

        printf("\nEnter string (type 'exit' to stop): ");
        scanf("%s",buffer);

        // Send string
        write(sock, buffer, sizeof(buffer));

        if(strcmp(buffer,"exit")==0)
            break;

        // Receive permutations
        read(sock, result, sizeof(result));

        printf("\nPermutations received:\n%s\n",result);
    }

    close(sock);
}
