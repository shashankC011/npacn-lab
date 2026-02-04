/*
    TCP SERVER PROGRAM
    -------------------
    This server accepts a set of integers from the client and performs:
    1. Search for a number
    2. Sort ascending
    3. Sort descending
    4. Split into odd and even
    5. Exit

    Compile: gcc server.c -o server
    Run: ./server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 100

// Function to sort ascending
void sortAscending(int arr[], int n){
    int i, j, temp;
    for(i=0;i<n-1;i++){
        for(j=0;j<n-i-1;j++){
            if(arr[j] > arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Function to sort descending
void sortDescending(int arr[], int n){
    int i, j, temp;
    for(i=0;i<n-1;i++){
        for(j=0;j<n-i-1;j++){
            if(arr[j] < arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int arr[MAX], n, choice, key, i;

    // Step 1: Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd == 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Define server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Step 3: Bind socket
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Step 4: Listen for connections
    listen(server_fd, 3);
    printf("Server is waiting for connection...\n");

    // Step 5: Accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Client connected!\n");

    while(1){

        // Receive choice
        read(new_socket, &choice, sizeof(choice));

        if(choice == 5){
            printf("Client exited.\n");
            break;
        }

        // Receive number of elements
        read(new_socket, &n, sizeof(n));

        // Receive array
        read(new_socket, arr, sizeof(int)*n);

        switch(choice){

            case 1: // Search
                read(new_socket, &key, sizeof(key));

                int found = 0;
                for(i=0;i<n;i++){
                    if(arr[i] == key){
                        found = 1;
                        break;
                    }
                }

                write(new_socket, &found, sizeof(found));
                break;

            case 2: // Sort Ascending
                sortAscending(arr, n);
                write(new_socket, arr, sizeof(int)*n);
                break;

            case 3: // Sort Descending
                sortDescending(arr, n);
                write(new_socket, arr, sizeof(int)*n);
                break;

            case 4: // Odd Even split
                int even[MAX], odd[MAX];
                int e=0, o=0;

                for(i=0;i<n;i++){
                    if(arr[i] % 2 == 0)
                        even[e++] = arr[i];
                    else
                        odd[o++] = arr[i];
                }

                // send counts first
                write(new_socket, &e, sizeof(e));
                write(new_socket, even, sizeof(int)*e);

                write(new_socket, &o, sizeof(o));
                write(new_socket, odd, sizeof(int)*o);

                break;
        }
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
