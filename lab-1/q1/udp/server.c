/*
    UDP SERVER PROGRAM
    ------------------
    Receives integers and performs:
    1. Search
    2. Sort Ascending
    3. Sort Descending
    4. Odd-Even Split
    5. Exit

    Compile: gcc server_udp.c -o server
    Run: ./server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 100

// Sorting functions
void sortAsc(int arr[], int n){
    int i,j,temp;
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

void sortDesc(int arr[], int n){
    int i,j,temp;
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

    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    int arr[MAX], n, choice, key, i;

    // Step 1: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Step 2: Define server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Step 3: Bind
    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    printf("UDP Server is running...\n");

    addr_size = sizeof(clientAddr);

    while(1){

        // Receive choice
        recvfrom(sockfd, &choice, sizeof(choice), 0,
                (struct sockaddr*)&clientAddr, &addr_size);

        if(choice == 5){
            printf("Client exited.\n");
            break;
        }

        // Receive array size
        recvfrom(sockfd, &n, sizeof(n), 0,
                (struct sockaddr*)&clientAddr, &addr_size);

        // Receive array
        recvfrom(sockfd, arr, sizeof(int)*n, 0,
                (struct sockaddr*)&clientAddr, &addr_size);

        switch(choice){

            case 1: // Search
            {
                recvfrom(sockfd, &key, sizeof(key), 0,
                        (struct sockaddr*)&clientAddr, &addr_size);

                int found = 0;
                for(i=0;i<n;i++){
                    if(arr[i] == key){
                        found = 1;
                        break;
                    }
                }

                sendto(sockfd, &found, sizeof(found), 0,
                        (struct sockaddr*)&clientAddr, addr_size);
                break;
            }

            case 2: // Sort Asc
                sortAsc(arr,n);
                sendto(sockfd, arr, sizeof(int)*n, 0,
                        (struct sockaddr*)&clientAddr, addr_size);
                break;

            case 3: // Sort Desc
                sortDesc(arr,n);
                sendto(sockfd, arr, sizeof(int)*n, 0,
                        (struct sockaddr*)&clientAddr, addr_size);
                break;

            case 4: // Odd Even
            {
                int even[MAX], odd[MAX];
                int e=0,o=0;

                for(i=0;i<n;i++){
                    if(arr[i]%2==0)
                        even[e++] = arr[i];
                    else
                        odd[o++] = arr[i];
                }

                sendto(sockfd, &e, sizeof(e), 0,
                        (struct sockaddr*)&clientAddr, addr_size);
                sendto(sockfd, even, sizeof(int)*e, 0,
                        (struct sockaddr*)&clientAddr, addr_size);

                sendto(sockfd, &o, sizeof(o), 0,
                        (struct sockaddr*)&clientAddr, addr_size);
                sendto(sockfd, odd, sizeof(int)*o, 0,
                        (struct sockaddr*)&clientAddr, addr_size);

                break;
            }
        }
    }

    close(sockfd);
    return 0;
}
