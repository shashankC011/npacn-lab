/*
    PROGRAM: TCP File Server

    DESCRIPTION:
    This server implements a file server using TCP sockets.
    The client sends the name of a text file to the server.

    ✔ If the file exists:
        - Server reads file contents
        - Calculates:
            -> File size
            -> Number of alphabets
            -> Number of digits
            -> Number of spaces
            -> Number of lines
            -> Number of special characters
        - Sends all results to the client

    ✔ If file does NOT exist:
        - Server sends "File not present"

    ✔ Server continues until client sends "stop"

    WHY TCP?
    Because file transfer requires reliability and ordered delivery.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 5000

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char filename[100];
    char buffer[MAX];

    // STEP 1: Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Define server properties
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept from any IP
    address.sin_port = htons(PORT);

    // STEP 2: Bind socket with IP + PORT
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // STEP 3: Listen for client connections
    listen(server_fd, 3);

    printf("File Server is running...\n");

    // STEP 4: Accept client
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    while(1){

        // Receive filename from client
        read(new_socket, filename, sizeof(filename));

        // Stop condition
        if(strcmp(filename,"stop")==0){
            printf("Client terminated.\n");
            break;
        }

        FILE *fp = fopen(filename,"r");

        // If file not found
        if(fp == NULL){

            strcpy(buffer,"File not present");
            write(new_socket, buffer, sizeof(buffer));
            continue; // wait for next filename
        }

        // Inform client file exists
        strcpy(buffer,"File present");
        write(new_socket, buffer, sizeof(buffer));


        // VARIABLES FOR STATISTICS
        int alphabets=0, digits=0, spaces=0;
        int lines=0, special=0;
        int size=0;

        char fileData[MAX];
        int index = 0;
        char ch;

        // Read file character by character
        while((ch = fgetc(fp)) != EOF){

            fileData[index++] = ch;
            size++;

            if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
                alphabets++;

            else if(ch>='0' && ch<='9')
                digits++;

            else if(ch==' ')
                spaces++;

            else if(ch=='\n')
                lines++;

            else
                special++;
        }

        fileData[index] = '\0';

        fclose(fp);

        // Send file content FIRST
        write(new_socket, fileData, sizeof(fileData));

        // Prepare statistics message
        sprintf(buffer,
        "\nFILE STATISTICS:\n"
        "File Size: %d bytes\n"
        "Alphabets: %d\n"
        "Digits: %d\n"
        "Spaces: %d\n"
        "Lines: %d\n"
        "Special Characters: %d\n",
        size, alphabets, digits, spaces, lines, special);

        // Send statistics
        write(new_socket, buffer, sizeof(buffer));
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
