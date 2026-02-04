/*
    TCP BOOK DATABASE CLIENT
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9090

struct Book{
    char title[50];
    char author[50];
    int accession;
    int pages;
    char publisher[50];
};

int main(){

    int sock, choice, type;
    struct sockaddr_in serv_addr;

    struct Book b;
    char buffer[2000];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0){
        perror("Socket failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server!\n");

    while(1){

        printf("\n1.Insert\n2.Delete\n3.Display\n4.Search\n5.Exit");
        printf("\nEnter choice: ");
        scanf("%d",&choice);

        write(sock,&choice,sizeof(choice));

        if(choice==5) break;

        // INSERT
        if(choice==1){

            printf("Enter Title: ");
            scanf(" %[^\n]",b.title);

            printf("Enter Author: ");
            scanf(" %[^\n]",b.author);

            printf("Enter Accession: ");
            scanf("%d",&b.accession);

            printf("Enter Pages: ");
            scanf("%d",&b.pages);

            printf("Enter Publisher: ");
            scanf(" %[^\n]",b.publisher);

            write(sock,&b,sizeof(b));
        }

        // DELETE
        else if(choice==2){

            int acc;
            printf("Enter accession number: ");
            scanf("%d",&acc);

            write(sock,&acc,sizeof(acc));
        }

        // SEARCH
        else if(choice==4){

            printf("1.Search by Title\n2.Search by Author\n");
            scanf("%d",&type);

            write(sock,&type,sizeof(type));

            if(type==1){
                char title[50];
                printf("Enter title: ");
                scanf(" %[^\n]",title);
                write(sock,title,sizeof(title));
            }
            else{
                char author[50];
                printf("Enter author: ");
                scanf(" %[^\n]",author);
                write(sock,author,sizeof(author));
            }
        }

        read(sock,buffer,sizeof(buffer));
        printf("\nSERVER RESPONSE:\n%s\n",buffer);
    }

    close(sock);
}
