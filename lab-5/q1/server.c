// PROGRAM: Concurrent Server with Multiple Clients using TCP

// DESCRIPTION:
// This program demonstrates a concurrent server handling multiple clients.

// Client1 sends "Institute Of".
// Client2 sends "Technology".

// The server appends these to a file containing "Manipal"
// and displays:

// "Manipal Institute Of Technology"

// along with the socket addresses of both clients.

// If more than two clients connect, the server sends
// "terminate session" to all clients and exits.

/*
    CONCURRENT TCP SERVER

    Demonstrates:
    ✔ Multiple clients
    ✔ fork()
    ✔ File operations
    ✔ Client socket address
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 9090

int client_count = 0;
int client_sockets[10]; // store sockets

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    char buffer[1024];

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

    printf("Concurrent Server Running...\n");

    // create file with "Manipal"
    FILE *fp = fopen("college.txt","w");
    fprintf(fp,"Manipal");
    fclose(fp);

    while(1){

        new_socket = accept(server_fd,
                    (struct sockaddr*)&client_addr,
                    &addrlen);

        if(new_socket < 0){
            perror("Accept failed");
            continue;
        }

        client_sockets[client_count++] = new_socket;

        printf("Client connected: %s : %d\n",
            inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));

        // TERMINATE if >2
        if(client_count > 2){

            for(int i=0;i<client_count;i++){
                write(client_sockets[i],
                      "terminate session",
                      strlen("terminate session")+1);
            }

            printf("More than 2 clients. Terminating...\n");
            exit(0);
        }

        // FORK for concurrency
        pid_t pid = fork();

        if(pid==0){ // CHILD

            char msg[100];

            read(new_socket,msg,sizeof(msg));

            // Append to file
            FILE *fp = fopen("college.txt","a");
            fprintf(fp," %s",msg);
            fclose(fp);

            printf("Received from %s:%d -> %s\n",
                inet_ntoa(client_addr.sin_addr),
                ntohs(client_addr.sin_port),
                msg);

            // After second client, display result
            if(client_count==2){

                FILE *fp = fopen("college.txt","r");
                fgets(buffer,sizeof(buffer),fp);
                fclose(fp);

                printf("\nFINAL STRING: %s\n",buffer);

                printf("Client Addresses:\n");

                for(int i=0;i<2;i++){
                    printf("Client %d socket: %d\n",
                        i+1, client_sockets[i]);
                }
            }

            exit(0);
        }
    }

    close(server_fd);
}
