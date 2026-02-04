/*
    PROGRAM: Half Duplex Chat Server using TCP

    DESCRIPTION:
    This server demonstrates half duplex communication using
    connection-oriented TCP sockets.

    WORKING:
    - Client sends a string.
    - Server generates all permutations of the string.
    - Server sends permutations back to the client.
    - Process continues until client sends "exit".

    NOTE:
    fork() is NOT used because communication is NOT simultaneous.
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 5000


// Swap function used in permutation
void swap(char *x, char *y){
    char temp = *x;
    *x = *y;
    *y = temp;
}


// Recursive permutation generator
void permute(char *str, int l, int r, char *result){

    // Base condition -> one permutation ready
    if(l == r){
        strcat(result, str);
        strcat(result, "\n");
        return;
    }

    for(int i=l;i<=r;i++){

        swap(&str[l], &str[i]);        // Fix one character

        permute(str, l+1, r, result);  // Recurse

        swap(&str[l], &str[i]);        // Backtrack
    }
}


int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[100];
    char result[MAX];

    // STEP 1: Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // STEP 2: Bind
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // STEP 3: Listen
    listen(server_fd, 3);

    printf("TCP Half Duplex Server waiting...\n");

    // STEP 4: Accept client
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    while(1){

        // Receive string from client
        read(new_socket, buffer, sizeof(buffer));

        if(strcmp(buffer,"exit")==0){
            printf("Client terminated.\n");
            break;
        }

        result[0] = '\0'; // clear previous data

        // Generate permutations
        permute(buffer, 0, strlen(buffer)-1, result);

        // Send permutations
        write(new_socket, result, sizeof(result));
    }

    close(new_socket);
    close(server_fd);
}
