/*
    PROGRAM: TCP File Operations Server

    DESCRIPTION:
    The server receives a filename from the client. If the file exists,
    it allows the client to perform operations such as searching a string,
    replacing a string, and reordering the file contents in ASCII order.
    If the file is not present, the server informs the client and terminates.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 2000

// Count occurrences of substring
int countOccurrences(char *data, char *word){
    int count = 0;
    char *pos = data;

    while((pos = strstr(pos, word)) != NULL){
        count++;
        pos += strlen(word);
    }
    return count;
}

// Replace string in file
int replaceString(char *data, char *str1, char *str2, char *result){
    char *pos;
    int found = 0;

    result[0] = '\0';

    while((pos = strstr(data, str1)) != NULL){
        found = 1;
        strncat(result, data, pos-data);
        strcat(result, str2);
        data = pos + strlen(str1);
    }
    strcat(result, data);

    return found;
}

// Reorder file content
void reorder(char *data){
    int len = strlen(data);

    for(int i=0;i<len-1;i++){
        for(int j=0;j<len-i-1;j++){
            if(data[j] > data[j+1]){
                char t = data[j];
                data[j] = data[j+1];
                data[j+1] = t;
            }
        }
    }
}

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char filename[100], buffer[MAX];
    int choice;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server waiting...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    // Receive filename
    read(new_socket, filename, sizeof(filename));

    FILE *fp = fopen(filename,"r");

    if(fp == NULL){
        strcpy(buffer,"File not present");
        write(new_socket,buffer,sizeof(buffer));
        close(new_socket);
        return 0;
    }

    strcpy(buffer,"File present");
    write(new_socket,buffer,sizeof(buffer));

    while(1){

        read(new_socket,&choice,sizeof(choice));

        if(choice==4){
            printf("Client exited.\n");
            break;
        }

        // Read file content
        fp = fopen(filename,"r");
        fread(buffer,1,MAX,fp);
        fclose(fp);

        if(choice==1){ // SEARCH

            char word[100];
            read(new_socket,word,sizeof(word));

            int count = countOccurrences(buffer,word);

            if(count>0)
                sprintf(buffer,"Occurrences: %d",count);
            else
                strcpy(buffer,"String not found");

            write(new_socket,buffer,sizeof(buffer));
        }

        else if(choice==2){ // REPLACE

            char str1[100],str2[100];
            char result[MAX];

            read(new_socket,str1,sizeof(str1));
            read(new_socket,str2,sizeof(str2));

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

            write(new_socket,buffer,sizeof(buffer));
        }

        else if(choice==3){ // REORDER

            reorder(buffer);

            fp = fopen(filename,"w");
            fwrite(buffer,1,strlen(buffer),fp);
            fclose(fp);

            strcpy(buffer,"File reordered in ASCII order");
            write(new_socket,buffer,sizeof(buffer));
        }
    }

    close(new_socket);
    close(server_fd);
}
