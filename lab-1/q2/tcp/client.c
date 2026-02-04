/*
    TCP CLIENT PROGRAM
    -------------------
    Aim:
    To send a string to the TCP server and receive the processed
    output including palindrome status, string length, and vowel count.

    Algorithm:
    1. Create a TCP socket.
    2. Connect to the server.
    3. Enter a string from the user.
    4. Send it to the server using write().
    5. Receive the result using read().
    6. Display the response.
    7. Repeat until "Halt" is entered.
    8. Close the socket.

    Compile: gcc client_tcp.c -o client
    Run: ./client
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

    sock = socket(AF_INET,SOCK_STREAM,0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    while(1){

        printf("\nEnter string (Halt to stop): ");
        fgets(buffer,MAX,stdin);
        buffer[strcspn(buffer,"\n")] = 0;

        write(sock,buffer,strlen(buffer)+1);

        if(strcmp(buffer,"Halt")==0)
            break;

        read(sock,buffer,MAX);

        printf("\nServer Response:\n%s\n",buffer);
    }

    close(sock);
}
