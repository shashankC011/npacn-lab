/*
    PROGRAM: TCP Client-Server using fork()

    DESCRIPTION:
    The client sends an alphanumeric string to the server.

    Server creates TWO processes using fork():

    CHILD PROCESS:
        - Extracts digits
        - Sorts them in ASCENDING order
        - Sends result with its PID

    PARENT PROCESS:
        - Extracts alphabets
        - Sorts them in DESCENDING order
        - Sends result with its PID

    Demonstrates multiprocessing + TCP sockets.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024


// Sort ascending (Bubble sort)
void sortAscending(char arr[], int n){
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j] > arr[j+1]){
                char t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
        }
    }
}


// Sort descending
void sortDescending(char arr[], int n){
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j] < arr[j+1]){
                char t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
        }
    }
}


int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[MAX];

    // STEP 1: Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // STEP 2: Bind
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // STEP 3: Listen
    listen(server_fd, 3);

    printf("Server waiting...\n");

    // STEP 4: Accept
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    // Receive string from client
    read(new_socket, buffer, sizeof(buffer));

    printf("Input string: %s\n",buffer);

    pid_t pid = fork();

    if(pid == 0){
        // CHILD PROCESS -> HANDLE DIGITS

        char digits[100];
        int k=0;

        // Extract digits
        for(int i=0;i<strlen(buffer);i++){
            if(buffer[i] >= '0' && buffer[i] <= '9'){
                digits[k++] = buffer[i];
            }
        }

        digits[k] = '\0';

        // Sort ascending
        sortAscending(digits, k);

        char result[MAX];

        sprintf(result,
        "Output at CHILD (PID %d): %s",
        getpid(), digits);

        // Send to client
        write(new_socket, result, sizeof(result));

        exit(0);
    }
    else{
        // PARENT PROCESS -> HANDLE ALPHABETS

        char alphabets[100];
        int k=0;

        // Extract letters
        for(int i=0;i<strlen(buffer);i++){
            if((buffer[i]>='A' && buffer[i]<='Z') ||
               (buffer[i]>='a' && buffer[i]<='z')){
                alphabets[k++] = buffer[i];
            }
        }

        alphabets[k] = '\0';

        // Sort descending
        sortDescending(alphabets, k);

        char result[MAX];

        sprintf(result,
        "Output at PARENT (PID %d): %s",
        getpid(), alphabets);

        // Send to client
        write(new_socket, result, sizeof(result));
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
