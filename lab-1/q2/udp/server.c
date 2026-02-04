/*
    UDP Palindrome SERVER PROGRAM

    -------------------
    Aim:
    To implement a UDP client-server model where the client sends a string
    to the server and the server checks whether it is a palindrome.
    The server also calculates the length of the string and counts
    the occurrence of each vowel (A, E, I, O, U), then sends the result
    back to the client.

    Algorithm:
    1. Create a UDP socket.
    2. Bind the socket to a port number.
    3. Wait for the client message using recvfrom().
    4. Check if the string is palindrome.
    5. Calculate string length.
    6. Count vowels.
    7. Send the result back using sendto().
    8. Repeat until the client sends "Halt".
    9. Close the socket.

    Compile: gcc server_udp.c -o server
    Run: ./server
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 1024

int isPalindrome(char str[]){
    int i=0, j=strlen(str)-1;

    while(i<j){
        if(str[i] != str[j])
            return 0;
        i++; j--;
    }
    return 1;
}

int main(){

    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    char buffer[MAX];
    char result[MAX];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

    printf("UDP Server Running...\n");

    addr_size = sizeof(clientAddr);

    while(1){

        recvfrom(sockfd, buffer, MAX, 0,
                (struct sockaddr*)&clientAddr, &addr_size);

        if(strcmp(buffer,"Halt")==0){
            printf("Server shutting down...\n");
            break;
        }

        int len = strlen(buffer);
        int a=0,e=0,i=0,o=0,u=0;

        for(int k=0;k<len;k++){
            char ch = buffer[k];

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

        sendto(sockfd,result,strlen(result)+1,0,
                (struct sockaddr*)&clientAddr,addr_size);
    }

    close(sockfd);
}
