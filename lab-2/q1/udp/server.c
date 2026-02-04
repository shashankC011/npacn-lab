/*
    PROGRAM: UDP File Operations Server

    DESCRIPTION:
    This server uses connectionless UDP sockets. The client sends a filename.
    If the file exists, the server allows operations such as:
        1. Search a string
        2. Replace a string
        3. Reorder file contents in ASCII order
        4. Exit

    If the file is not present, the server informs the client and terminates.

    NOTE:
    UDP does NOT require listen() or accept() because it is connectionless.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 2000


// Function to count occurrences of a substring inside file data
int countOccurrences(char *data, char *word){
    int count = 0;
    char *pos = data;

    // strstr finds first occurrence of word in data
    while((pos = strstr(pos, word)) != NULL){
        count++;
        pos += strlen(word); // move pointer forward
    }
    return count;
}


// Replace str1 with str2 inside data
int replaceString(char *data, char *str1, char *str2, char *result){

    char *pos;
    int found = 0;
    result[0] = '\0';

    while((pos = strstr(data, str1)) != NULL){

        found = 1;

        // Copy text BEFORE str1
        strncat(result, data, pos - data);

        // Append replacement string
        strcat(result, str2);

        // Move data pointer ahead
        data = pos + strlen(str1);
    }

    // Copy remaining data
    strcat(result, data);

    return found;
}


// Sort entire file content using Bubble Sort (ASCII based)
void reorder(char *data){

    int len = strlen(data);

    for(int i=0;i<len-1;i++){
        for(int j=0;j<len-i-1;j++){

            if(data[j] > data[j+1]){

                char temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}


int main(){

    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    char buffer[MAX];
    char filename[100];
    int choice;

    // STEP 1: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Define server properties
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // STEP 2: Bind socket
    bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

    printf("UDP Server Running...\n");

    addr_size = sizeof(clientAddr);

    // STEP 3: Receive filename from client
    recvfrom(sockfd, filename, sizeof(filename), 0,
            (struct sockaddr*)&clientAddr, &addr_size);

    FILE *fp = fopen(filename,"r");

    if(fp == NULL){
        strcpy(buffer,"File not present");

        sendto(sockfd, buffer, sizeof(buffer), 0,
                (struct sockaddr*)&clientAddr, addr_size);

        close(sockfd);
        return 0;
    }

    strcpy(buffer,"File present");

    sendto(sockfd, buffer, sizeof(buffer), 0,
            (struct sockaddr*)&clientAddr, addr_size);


    // MENU LOOP
    while(1){

        // Receive user choice
        recvfrom(sockfd, &choice, sizeof(choice), 0,
                (struct sockaddr*)&clientAddr, &addr_size);

        if(choice==4){
            printf("Client terminated.\n");
            break;
        }

        // Always reload file to get latest content
        fp = fopen(filename,"r");
        fread(buffer,1,MAX,fp);
        fclose(fp);

        if(choice==1){ // SEARCH

            char word[100];

            recvfrom(sockfd, word, sizeof(word), 0,
                    (struct sockaddr*)&clientAddr, &addr_size);

            int count = countOccurrences(buffer,word);

            if(count>0)
                sprintf(buffer,"Occurrences: %d",count);
            else
                strcpy(buffer,"String not found");

            sendto(sockfd, buffer, sizeof(buffer), 0,
                    (struct sockaddr*)&clientAddr, addr_size);
        }


        else if(choice==2){ // REPLACE

            char str1[100],str2[100];
            char result[MAX];

            recvfrom(sockfd, str1, sizeof(str1), 0,
                    (struct sockaddr*)&clientAddr, &addr_size);

            recvfrom(sockfd, str2, sizeof(str2), 0,
                    (struct sockaddr*)&clientAddr, &addr_size);

            int found = replaceString(buffer,str1,str2,result);

            if(found){

                fp = fopen(filename,"w");
                fwrite(result,1,strlen(result),fp);
                fclose(fp);

                strcpy(buffer,"String replaced");
            }
            else{
                strcpy(buffer,"String not found");
            }

            sendto(sockfd, buffer, sizeof(buffer), 0,
                    (struct sockaddr*)&clientAddr, addr_size);
        }


        else if(choice==3){ // REORDER

            reorder(buffer);

            fp = fopen(filename,"w");
            fwrite(buffer,1,strlen(buffer),fp);
            fclose(fp);

            strcpy(buffer,"File reordered in ASCII order");

            sendto(sockfd, buffer, sizeof(buffer), 0,
                    (struct sockaddr*)&clientAddr, addr_size);
        }
    }

    close(sockfd);
}
