/*
    TCP CLIENT PROGRAM
    -------------------
    Sends integers and a choice to the server.
    Continues until user selects EXIT.

    Compile: gcc client.c -o client
    Run: ./client
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 100

int main(){

    int sock = 0;
    struct sockaddr_in serv_addr;

    int arr[MAX], n, choice, key, i;

    // Step 1: Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Step 2: Connect to server
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while(1){

        printf("\nMENU\n");
        printf("1.Search\n2.Sort Asc\n3.Sort Desc\n4.Odd-Even Split\n5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        // Send choice
        write(sock, &choice, sizeof(choice));

        if(choice == 5)
            break;

        printf("Enter number of elements: ");
        scanf("%d", &n);

        printf("Enter elements:\n");
        for(i=0;i<n;i++)
            scanf("%d", &arr[i]);

        // send n and array
        write(sock, &n, sizeof(n));
        write(sock, arr, sizeof(int)*n);

        switch(choice){

            case 1:
                printf("Enter number to search: ");
                scanf("%d", &key);

                write(sock, &key, sizeof(key));

                int found;
                read(sock, &found, sizeof(found));

                if(found)
                    printf("Number FOUND!\n");
                else
                    printf("Number NOT FOUND!\n");
                break;

            case 2:
            case 3:
                read(sock, arr, sizeof(int)*n);

                printf("Result:\n");
                for(i=0;i<n;i++)
                    printf("%d ", arr[i]);
                printf("\n");
                break;

            case 4:
                int e,o;
                int even[MAX], odd[MAX];

                read(sock, &e, sizeof(e));
                read(sock, even, sizeof(int)*e);

                read(sock, &o, sizeof(o));
                read(sock, odd, sizeof(int)*o);

                printf("Even numbers: ");
                for(i=0;i<e;i++)
                    printf("%d ", even[i]);

                printf("\nOdd numbers: ");
                for(i=0;i<o;i++)
                    printf("%d ", odd[i]);

                printf("\n");
                break;
        }
    }

    close(sock);
    return 0;
}
