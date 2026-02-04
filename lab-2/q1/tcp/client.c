/*
    PROGRAM: TCP File Operations Client

    DESCRIPTION:
    The client sends a filename to the server and performs operations
    like search, replace, and reorder based on user choice until exit.
*/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 2000

int main(){

    int sock;
    struct sockaddr_in serv_addr;

    char buffer[MAX];
    char filename[100];
    int choice;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    // Send filename
    printf("Enter file name: ");
    scanf("%s",filename);

    write(sock,filename,sizeof(filename));

    read(sock,buffer,sizeof(buffer));

    if(strcmp(buffer,"File not present")==0){
        printf("Server: File not present\n");
        close(sock);
        return 0;
    }

    while(1){

        printf("\n1.Search\n2.Replace\n3.Reorder\n4.Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        write(sock,&choice,sizeof(choice));

        if(choice==4)
            break;

        if(choice==1){
            char word[100];
            printf("Enter string to search: ");
            scanf("%s",word);

            write(sock,word,sizeof(word));
        }

        else if(choice==2){
            char str1[100],str2[100];

            printf("Enter string to replace: ");
            scanf("%s",str1);

            printf("Enter new string: ");
            scanf("%s",str2);

            write(sock,str1,sizeof(str1));
            write(sock,str2,sizeof(str2));
        }

        read(sock,buffer,sizeof(buffer));
        printf("Server: %s\n",buffer);
    }

    close(sock);
}
