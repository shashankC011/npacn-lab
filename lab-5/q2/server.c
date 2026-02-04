// PROGRAM: Multiple Client - Single Iterative Server using TCP

// DESCRIPTION:
// This program demonstrates multiple clients communicating with a
// single iterative server.

// Each client sends two strings to the server along with its socket address.
// The server checks whether the strings are anagrams and sends the result back.

// The server also displays the current date and time along with
// the client socket address whenever a connection is established.

// Since the server is iterative, it handles one client at a time.

/*
    ITERATIVE TCP SERVER - ANAGRAM CHECK

    Features:
    ✔ Handles multiple clients sequentially
    ✔ Displays client socket address
    ✔ Shows date & time
    ✔ Checks anagrams
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 9090

// Function to check anagram
int isAnagram(char s1[], char s2[]){

    int count[256] = {0};

    if(strlen(s1) != strlen(s2))
        return 0;

    for(int i=0;i<strlen(s1);i++){
        count[s1[i]]++;
        count[s2[i]]--;
    }

    for(int i=0;i<256;i++){
        if(count[i] != 0)
            return 0;
    }

    return 1;
}

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    char str1[100], str2[100], result[200];

    // SOCKET
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket failed");
        exit(1);
    }

    int opt=1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address))<0){
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd,5);

    printf("Iterative Server Running...\n");

    while(1){

        // ACCEPT CLIENT
        new_socket = accept(server_fd,
                        (struct sockaddr*)&client_addr,
                        &addrlen);

        if(new_socket < 0){
            perror("Accept failed");
            continue;
        }

        // DISPLAY CLIENT ADDRESS
        printf("\nConnected to Client: %s : %d\n",
            inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));

        // DISPLAY DATE & TIME
        time_t now;
        time(&now);
        printf("Connection Time: %s",ctime(&now));

        // READ STRINGS
        read(new_socket,str1,sizeof(str1));
        read(new_socket,str2,sizeof(str2));

        printf("Received strings: %s , %s\n",str1,str2);

        // CHECK ANAGRAM
        if(isAnagram(str1,str2))
            strcpy(result,"Strings are ANAGRAMS.");
        else
            strcpy(result,"Strings are NOT anagrams.");

        // SEND RESULT
        write(new_socket,result,sizeof(result));

        // CLOSE CLIENT (KEY STEP FOR ITERATIVE)
        close(new_socket);

        printf("Client processed. Waiting for next...\n");
    }

    close(server_fd);
}
