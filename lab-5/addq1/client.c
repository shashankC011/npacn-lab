/*
    TRAVEL TICKET CLIENT
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9090

struct Route{
    char source[50];
    char dest[50];
    int total;
    int available;
};

int main(){

    int sock;
    struct sockaddr_in serv_addr;

    struct Route routes[2];
    char buffer[500];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0){
        perror("Socket failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to reservation server.\n");

    // Receive routes
    read(sock,routes,sizeof(routes));

    printf("\nAvailable Routes:\n");

    for(int i=0;i<2;i++){
        printf("%d. %s -> %s | Available Seats: %d\n",
            i+1,
            routes[i].source,
            routes[i].dest,
            routes[i].available);
    }

    int choice, seats;

    printf("Select route: ");
    scanf("%d",&choice);

    printf("Enter seats to book: ");
    scanf("%d",&seats);

    write(sock,&choice,sizeof(choice));
    write(sock,&seats,sizeof(seats));

    read(sock,buffer,sizeof(buffer));

    printf("\nSERVER RESPONSE:\n%s\n",buffer);

    close(sock);
}
