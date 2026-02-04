/*
    PROGRAM: Half Duplex Chat Server using UDP

    DESCRIPTION:
    This program demonstrates a half duplex communication using
    connectionless UDP sockets.

    Workflow:
    1. Client sends a string.
    2. Server generates ALL permutations of the string.
    3. Server sends permutations back to client.
    4. Process repeats until client sends "exit".

    Half Duplex:
    Only one side communicates at a time.
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 2000


// Function to swap characters
void swap(char *x, char *y){
    char temp = *x;
    *x = *y;
    *y = temp;
}


// Recursive function to generate permutations
void permute(char *str, int l, int r, char *result){

    if(l == r){
        strcat(result, str);   // append permutation
        strcat(result, "\n");  // new line
        return;
    }

    for(int i=l;i<=r;i++){

        swap(&str[l], &str[i]);

        permute(str, l+1, r, result);

        swap(&str[l], &str[i]); // backtrack
    }
}


int main(){

    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    char buffer[100];
    char result[MAX];

    // STEP 1: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // STEP 2: Bind socket
    bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

    printf("UDP Half Duplex Server Running...\n");

    addr_size = sizeof(clientAddr);

    while(1){

        // Receive string from client
        recvfrom(sockfd, buffer, sizeof(buffer), 0,
                (struct sockaddr*)&clientAddr, &addr_size);

        if(strcmp(buffer,"exit")==0){
            printf("Client terminated.\n");
            break;
        }

        result[0] = '\0'; // clear result

        // Generate permutations
        permute(buffer, 0, strlen(buffer)-1, result);

        // Send permutations back
        sendto(sockfd, result, sizeof(result), 0,
                (struct sockaddr*)&clientAddr, addr_size);
    }

    close(sockfd);
}
