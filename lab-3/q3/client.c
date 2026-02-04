/*
    PROGRAM: TCP Client for Fork Demonstration

    DESCRIPTION:
    Client sends an alphanumeric string to the server.
    Receives TWO messages:
        - From child (digits sorted ascending)
        - From parent (alphabets sorted descending)
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    // STEP 2: Connect
    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    printf("Enter alphanumeric string: ");
    scanf("%s",buffer);

    // Send string
    write(sock, buffer, sizeof(buffer));

    // Receive CHILD output
    read(sock, buffer, sizeof(buffer));
    printf("%s\n",buffer);

    // Receive PARENT output
    read(sock, buffer, sizeof(buffer));
    printf("%s\n",buffer);

    close(sock);
}
