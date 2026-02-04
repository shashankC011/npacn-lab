/*
    TCP BOOK DATABASE SERVER
    -----------------------

    Maintains a file-based database of books.

    Operations:
    1. Insert
    2. Delete
    3. Display
    4. Search
    5. Exit

    Uses:
    ✔ TCP sockets
    ✔ Structures
    ✔ Binary file handling
    ✔ Proper error checking
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define FILE_NAME "books.dat"

// Structure for book record
struct Book{
    char title[50];
    char author[50];
    int accession;
    int pages;
    char publisher[50];
};

int main(){

    int server_fd, new_socket, choice;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    struct Book b;
    char buffer[2000];

    // ================= SOCKET =================
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0){
        perror("Socket failed");
        exit(1);
    }

    // Allows port reuse (VERY IMPORTANT after crashes)
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // ================= BIND =================
    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address)) < 0){
        perror("Bind failed");
        exit(1);
    }

    // ================= LISTEN =================
    if(listen(server_fd,3) < 0){
        perror("Listen failed");
        exit(1);
    }

    printf("Book Database Server Running on port %d...\n",PORT);

    // ================= ACCEPT =================
    new_socket = accept(server_fd,(struct sockaddr*)&client_addr,&addrlen);

    if(new_socket < 0){
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected successfully!\n");

    while(1){

        // Read client choice
        read(new_socket,&choice,sizeof(choice));

        if(choice==5){
            printf("Client exited.\n");
            break;
        }

        // ================= INSERT =================
        if(choice==1){

            FILE *fp = fopen(FILE_NAME,"ab");

            if(fp==NULL){
                strcpy(buffer,"File error!");
                write(new_socket,buffer,sizeof(buffer));
                continue;
            }

            read(new_socket,&b,sizeof(b));

            fwrite(&b,sizeof(b),1,fp);
            fclose(fp);

            strcpy(buffer,"Book inserted successfully.");
            write(new_socket,buffer,sizeof(buffer));
        }

        // ================= DELETE =================
        else if(choice==2){

            int acc,found=0;

            FILE *fp = fopen(FILE_NAME,"rb");

            if(fp==NULL){
                strcpy(buffer,"Database empty.");
                write(new_socket,buffer,sizeof(buffer));
                continue;
            }

            FILE *temp = fopen("temp.dat","wb");

            read(new_socket,&acc,sizeof(acc));

            while(fread(&b,sizeof(b),1,fp)){

                if(b.accession==acc){
                    found=1;
                    continue;
                }

                fwrite(&b,sizeof(b),1,temp);
            }

            fclose(fp);
            fclose(temp);

            remove(FILE_NAME);
            rename("temp.dat",FILE_NAME);

            if(found)
                strcpy(buffer,"Book deleted successfully.");
            else
                strcpy(buffer,"Book not found.");

            write(new_socket,buffer,sizeof(buffer));
        }

        // ================= DISPLAY =================
        else if(choice==3){

            FILE *fp = fopen(FILE_NAME,"rb");

            if(fp==NULL){
                strcpy(buffer,"No books in database.");
                write(new_socket,buffer,sizeof(buffer));
                continue;
            }

            buffer[0]='\0';

            while(fread(&b,sizeof(b),1,fp)){

                char temp[300];

                sprintf(temp,
                "\nTitle: %s\nAuthor: %s\nAccession: %d\nPages: %d\nPublisher: %s\n",
                b.title,b.author,b.accession,b.pages,b.publisher);

                strcat(buffer,temp);
            }

            fclose(fp);

            if(strlen(buffer)==0)
                strcpy(buffer,"No books available.");

            write(new_socket,buffer,sizeof(buffer));
        }

        // ================= SEARCH =================
        else if(choice==4){

            int type,found=0;

            read(new_socket,&type,sizeof(type));

            FILE *fp = fopen(FILE_NAME,"rb");

            if(fp==NULL){
                strcpy(buffer,"Database empty.");
                write(new_socket,buffer,sizeof(buffer));
                continue;
            }

            buffer[0]='\0';

            if(type==1){ // Search by title

                char title[50];
                read(new_socket,title,sizeof(title));

                while(fread(&b,sizeof(b),1,fp)){

                    if(strcmp(b.title,title)==0){

                        char temp[300];

                        sprintf(temp,
                        "\nTitle: %s\nAuthor: %s\nAccession: %d\nPages: %d\nPublisher: %s\n",
                        b.title,b.author,b.accession,b.pages,b.publisher);

                        strcat(buffer,temp);
                        found=1;
                    }
                }
            }
            else{ // Search by author

                char author[50];
                read(new_socket,author,sizeof(author));

                while(fread(&b,sizeof(b),1,fp)){

                    if(strcmp(b.author,author)==0){

                        char temp[300];

                        sprintf(temp,
                        "\nTitle: %s\nAuthor: %s\nAccession: %d\nPages: %d\nPublisher: %s\n",
                        b.title,b.author,b.accession,b.pages,b.publisher);

                        strcat(buffer,temp);
                        found=1;
                    }
                }
            }

            fclose(fp);

            if(!found)
                strcpy(buffer,"No matching book found.");

            write(new_socket,buffer,sizeof(buffer));
        }
    }

    close(new_socket);
    close(server_fd);
}
