// PROGRAM: DNS Server using TCP Socket

// DESCRIPTION:
// This program simulates a DNS (Domain Name System) server using
// connection-oriented TCP sockets.

// The client sends a domain name to the server.
// The server searches its database (implemented using structures)
// and returns the corresponding IP address.

// If the domain is not found, an appropriate message is sent.
// The process continues until the client enters "exit".

// Example: google.com → 142.250.183.206


/*
    PROGRAM: TCP DNS Server using Structures

    Demonstrates:
    ✔ TCP sockets
    ✔ Client-server communication
    ✔ Structure-based database
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 1024


// DNS STRUCTURE
struct DNS{
    char domain[50];
    char ip[20];
};


// DATABASE
struct DNS db[] = {

{"google.com","142.250.183.206"},
{"facebook.com","157.240.22.35"},
{"openai.com","104.18.33.45"},
{"youtube.com","142.250.190.14"}

};

int total = 4;


int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[MAX];
    char result[MAX];

    // STEP 1: Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // STEP 2: Bind
    bind(server_fd,(struct sockaddr*)&address,sizeof(address));

    // STEP 3: Listen
    listen(server_fd,3);

    printf("DNS Server Running...\n");

    // STEP 4: Accept
    new_socket = accept(server_fd,(struct sockaddr*)&address,&addrlen);

    while(1){

        // Receive domain name
        read(new_socket,buffer,sizeof(buffer));

        if(strcmp(buffer,"exit")==0){
            printf("Client terminated.\n");
            break;
        }

        int found = 0;

        // Search database
        for(int i=0;i<total;i++){

            if(strcmp(buffer,db[i].domain)==0){

                sprintf(result,
                "Domain: %s\nIP Address: %s",
                db[i].domain,db[i].ip);

                found = 1;
                break;
            }
        }

        if(!found){
            strcpy(result,"Domain not found in DNS database.");
        }

        // Send result
        write(new_socket,result,sizeof(result));
    }

    close(new_socket);
    close(server_fd);
}
