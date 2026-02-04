/*
    PROGRAM: Menu Driven Calculator Server using TCP

    DESCRIPTION:
    This server performs mathematical operations requested by the client.

    Operations supported:
    1. Addition and Subtraction
    2. Solve linear equation (ax + b = 0)
    3. Matrix multiplication
    4. Exit

    WHY TCP?
    Because calculator operations require reliable and ordered data transfer.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 10

int main(){

    int server_fd, new_socket, choice;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen
    listen(server_fd, 3);

    printf("Calculator Server Running...\n");

    // Accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    while(1){

        // Receive menu choice
        read(new_socket, &choice, sizeof(choice));

        if(choice == 4){
            printf("Client exited.\n");
            break;
        }

        // ================= ADD & SUBTRACT =================
        if(choice == 1){

            int a,b,result[2];

            read(new_socket,&a,sizeof(a));
            read(new_socket,&b,sizeof(b));

            result[0] = a + b; // addition
            result[1] = a - b; // subtraction

            write(new_socket,result,sizeof(result));
        }

        // ================= LINEAR EQUATION =================
        else if(choice == 2){

            float a,b,x;

            read(new_socket,&a,sizeof(a));
            read(new_socket,&b,sizeof(b));

            if(a == 0)
                x = 0; // avoid division by zero
            else
                x = -b/a;

            write(new_socket,&x,sizeof(x));
        }

        // ================= MATRIX MULTIPLICATION =================
        else if(choice == 3){

            int r1,c1,r2,c2;
            int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

            read(new_socket,&r1,sizeof(r1));
            read(new_socket,&c1,sizeof(c1));
            read(new_socket,&r2,sizeof(r2));
            read(new_socket,&c2,sizeof(c2));

            read(new_socket,A,sizeof(A));
            read(new_socket,B,sizeof(B));

            // Multiply matrices
            for(int i=0;i<r1;i++){
                for(int j=0;j<c2;j++){

                    C[i][j] = 0;

                    for(int k=0;k<c1;k++){
                        C[i][j] += A[i][k] * B[k][j];
                    }
                }
            }

            write(new_socket,C,sizeof(C));
            write(new_socket,&r1,sizeof(r1));
            write(new_socket,&c2,sizeof(c2));
        }
    }

    close(new_socket);
    close(server_fd);
}
