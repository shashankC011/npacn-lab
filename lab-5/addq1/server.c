// PROGRAM: Travel Ticket Reservation System using TCP

// DESCRIPTION:
// This program simulates a ticket reservation system where the
// server maintains seat availability for two routes.

// Multiple clients connect to the server and request seats.
// The server checks availability, updates its database,
// and sends confirmation or rejection messages.

// After successful booking or failure, the client terminates.

/*
    TRAVEL TICKET RESERVATION SERVER

    Demonstrates:
    ✔ Multiple clients
    ✔ fork() concurrency
    ✔ Shared database (structure)
    ✔ Seat updates
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090

// ROUTE STRUCTURE
struct Route{
    char source[50];
    char dest[50];
    int total;
    int available;
};

// DATABASE
struct Route routes[2] = {
    {"Manipal","Bangalore",40,40},
    {"Manipal","Delhi",30,30}
};

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);

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

    printf("Ticket Reservation Server Running...\n");

    while(1){

        new_socket = accept(server_fd,
                        (struct sockaddr*)&client_addr,
                        &addrlen);

        if(new_socket < 0){
            perror("Accept failed");
            continue;
        }

        printf("\nClient connected: %s:%d\n",
            inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));

        // CONCURRENT
        pid_t pid = fork();

        if(pid==0){ // CHILD

            int route_choice, seats;
            char buffer[500];

            // Display current seat status
            printf("\n---- Seat Status ----\n");

            for(int i=0;i<2;i++){
                printf("%d. %s -> %s | Available: %d | Booked: %d\n",
                    i+1,
                    routes[i].source,
                    routes[i].dest,
                    routes[i].available,
                    routes[i].total - routes[i].available);
            }

            // Send routes to client
            write(new_socket,routes,sizeof(routes));

            // Receive selection
            read(new_socket,&route_choice,sizeof(route_choice));
            read(new_socket,&seats,sizeof(seats));

            route_choice--; // convert to index

            if(seats <= routes[route_choice].available){

                routes[route_choice].available -= seats;

                sprintf(buffer,
                    "Booking SUCCESSFUL!\nSeats Reserved: %d\nRemaining Seats: %d",
                    seats,
                    routes[route_choice].available);

                write(new_socket,buffer,sizeof(buffer));

                printf("Seats booked successfully.\n");
            }
            else{

                strcpy(buffer,"Requested seats unavailable.");

                write(new_socket,buffer,sizeof(buffer));

                printf("Booking failed.\n");
            }

            close(new_socket);
            exit(0);
        }
    }

    close(server_fd);
}
