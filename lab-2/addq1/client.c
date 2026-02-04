/*
    PROGRAM: TCP File Client

    DESCRIPTION:
    The client sends filenames to the server.
    If the file exists, it receives:

        ✔ File contents
        ✔ File statistics

    Otherwise, it displays "File not present".

    Client repeats until user enters "stop".
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

    char filename[100];
    char buffer[MAX];

    // STEP 1: Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // STEP 2: Connect to server
    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    while(1){

        printf("\nEnter filename (type 'stop' to exit): ");
        scanf("%s",filename);

        // Send filename
        write(sock, filename, sizeof(filename));

        if(strcmp(filename,"stop")==0)
            break;

        // Check server response
        read(sock, buffer, sizeof(buffer));

        if(strcmp(buffer,"File not present")==0){

            printf("Server: File not present\n");
            continue;
        }

        // Receive file contents
        read(sock, buffer, sizeof(buffer));

        printf("\n------ FILE CONTENT ------\n");
        printf("%s\n",buffer);

        // Receive statistics
        read(sock, buffer, sizeof(buffer));

        printf("%s\n",buffer);
    }

    close(sock);
}
