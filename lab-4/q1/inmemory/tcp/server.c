// PROGRAM: Student Information System using TCP and Structures

// DESCRIPTION:
// The client sends either Registration Number, Student Name,
// or Subject Code to the server.

// The parent process creates a child process using fork()
// to handle each request.

// Student records are maintained using structures.
// The child searches the structure and sends the result
// along with its PID to the client.

/*
    TCP Server using Structures + fork()

    Demonstrates:
    ✔ Structures
    ✔ Multiprocessing
    ✔ TCP sockets
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX 1024


// STUDENT STRUCTURE
struct Student{
    char reg[20];
    char name[50];
    char address[100];
    char dept[50];
    char semester[10];
    char section[10];
    char courses[100];
    char subject[20];
    int marks;
};


// DATABASE (in-memory)
struct Student s[] = {

{"101","Rahul","Delhi","CSE","5","A","OS, CN, DBMS","CN",88},

{"102","Anita","Mumbai","ECE","4","B","Signals, Networks","Signals",91},

{"103","Karan","Bangalore","IT","6","A","AI, ML","AI",85}

};

int total = 3;


int main(){

    int server_fd, new_socket, option;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[MAX];
    char result[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd,(struct sockaddr*)&address,sizeof(address));

    listen(server_fd,3);

    printf("Student Server Running...\n");

    new_socket = accept(server_fd,(struct sockaddr*)&address,&addrlen);

    while(1){

        read(new_socket,&option,sizeof(option));

        if(option==4) break;

        read(new_socket,buffer,sizeof(buffer));

        pid_t pid = fork();

        if(pid==0){ // CHILD

            int found = 0;

            for(int i=0;i<total;i++){

                // OPTION 1 -> REGISTRATION NUMBER
                if(option==1 && strcmp(buffer,s[i].reg)==0){

                    sprintf(result,
                    "Handled by CHILD PID: %d\n"
                    "Name: %s\nAddress: %s",
                    getpid(),s[i].name,s[i].address);

                    found=1;
                    break;
                }

                // OPTION 2 -> NAME
                else if(option==2 && strcmp(buffer,s[i].name)==0){

                    sprintf(result,
                    "Handled by CHILD PID: %d\n"
                    "Dept: %s\nSemester: %s\nSection: %s\nCourses: %s",
                    getpid(),s[i].dept,s[i].semester,
                    s[i].section,s[i].courses);

                    found=1;
                    break;
                }

                // OPTION 3 -> SUBJECT
                else if(option==3 && strcmp(buffer,s[i].subject)==0){

                    sprintf(result,
                    "Handled by CHILD PID: %d\nMarks: %d",
                    getpid(),s[i].marks);

                    found=1;
                    break;
                }
            }

            if(!found){
                sprintf(result,"Record not found\nPID:%d",getpid());
            }

            write(new_socket,result,sizeof(result));

            return 0;
        }
        else{
            wait(NULL); // avoid zombie
        }
    }

    close(new_socket);
    close(server_fd);
}
