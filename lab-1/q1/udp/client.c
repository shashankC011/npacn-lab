/*
    UDP CLIENT PROGRAM
    ------------------
    Sends integers and choice to server until EXIT.
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 100

int main(){

    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    int arr[MAX], n, choice, key, i;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    addr_size = sizeof(serverAddr);

    while(1){

        printf("\n1.Search\n2.Sort Asc\n3.Sort Desc\n4.Odd-Even\n5.Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        sendto(sockfd, &choice, sizeof(choice), 0,
                (struct sockaddr*)&serverAddr, addr_size);

        if(choice==5)
            break;

        printf("Enter number of elements: ");
        scanf("%d",&n);

        printf("Enter elements:\n");
        for(i=0;i<n;i++)
            scanf("%d",&arr[i]);

        sendto(sockfd, &n, sizeof(n), 0,
                (struct sockaddr*)&serverAddr, addr_size);

        sendto(sockfd, arr, sizeof(int)*n, 0,
                (struct sockaddr*)&serverAddr, addr_size);

        switch(choice){

            case 1:
            {
                printf("Enter number to search: ");
                scanf("%d",&key);

                sendto(sockfd, &key, sizeof(key), 0,
                        (struct sockaddr*)&serverAddr, addr_size);

                int found;
                recvfrom(sockfd, &found, sizeof(found), 0,
                        NULL, NULL);

                if(found)
                    printf("FOUND\n");
                else
                    printf("NOT FOUND\n");

                break;
            }

            case 2:
            case 3:
                recvfrom(sockfd, arr, sizeof(int)*n, 0,
                        NULL, NULL);

                printf("Result:\n");
                for(i=0;i<n;i++)
                    printf("%d ", arr[i]);
                printf("\n");
                break;

            case 4:
            {
                int even[MAX], odd[MAX];
                int e,o;

                recvfrom(sockfd, &e, sizeof(e), 0,NULL,NULL);
                recvfrom(sockfd, even, sizeof(int)*e, 0,NULL,NULL);

                recvfrom(sockfd, &o, sizeof(o), 0,NULL,NULL);
                recvfrom(sockfd, odd, sizeof(int)*o, 0,NULL,NULL);

                printf("Even: ");
                for(i=0;i<e;i++) printf("%d ", even[i]);

                printf("\nOdd: ");
                for(i=0;i<o;i++) printf("%d ", odd[i]);

                printf("\n");
                break;
            }
        }
    }

    close(sockfd);
    return 0;
}
