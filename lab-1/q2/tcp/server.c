/*
    TCP PALINDROME SERVER PROGRAM
    -------------------
    Aim:
    To implement a TCP client-server model where the server receives
    a string from the client, checks whether it is a palindrome,
    calculates the length, counts vowels, and sends the result back.

    Algorithm:
    1. Create a TCP socket.
    2. Bind it to a port.
    3. Listen for incoming connections.
    4. Accept the client connection.
    5. Receive the string using read().
    6. Process palindrome, length, and vowel count.
    7. Send the result using write().
    8. Repeat until "Halt" is received.
    9. Close sockets.

    Compile: gcc server_tcp.c -o server
    Run: ./server
*/


#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 1024

int isPalindrome(char str[]){
    int i=0,j=strlen(str)-1;

    while(i<j){
        if(str[i]!=str[j])
            return 0;
        i++; j--;
    }
    return 1;
}

int main(){

    int server_fd,new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[MAX];
    char result[MAX];

    server_fd = socket(AF_INET,SOCK_STREAM,0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd,(struct sockaddr*)&address,sizeof(address));

    listen(server_fd,3);

    printf("TCP Server Waiting...\n");

    new_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen);

    while(1){

        read(new_socket,buffer,MAX);

        if(strcmp(buffer,"Halt")==0){
            printf("Server shutting down...\n");
            break;
        }

        int len=strlen(buffer);
        int a=0,e=0,i=0,o=0,u=0;

        for(int k=0;k<len;k++){
            char ch=buffer[k];

            if(ch=='a'||ch=='A') a++;
            else if(ch=='e'||ch=='E') e++;
            else if(ch=='i'||ch=='I') i++;
            else if(ch=='o'||ch=='O') o++;
            else if(ch=='u'||ch=='U') u++;
        }

        sprintf(result,
        "Palindrome: %s\nLength: %d\nVowels -> A:%d E:%d I:%d O:%d U:%d",
        isPalindrome(buffer)?"YES":"NO",
        len,a,e,i,o,u);

        write(new_socket,result,strlen(result)+1);
    }

    close(new_socket);
    close(server_fd);
}
