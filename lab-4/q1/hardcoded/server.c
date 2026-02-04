/*
    PROGRAM: Student Information Server using TCP and fork()

    DESCRIPTION:
    The client sends one of the following:
        1. Registration Number
        2. Student Name
        3. Subject Code

    The parent process assigns the task to a child process.

    CHILD 1 -> Sends Name + Address
    CHILD 2 -> Sends Enrollment Details
    CHILD 3 -> Sends Marks

    Each child sends its PID along with the result.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main(){

    int server_fd, new_socket, option;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[MAX];
    char result[MAX];

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 3);

    printf("Student Info Server Running...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    while(1){

        // Receive option
        read(new_socket, &option, sizeof(option));

        if(option == 4){
            printf("Client exited.\n");
            break;
        }

        // Receive data (reg no / name / subject code)
        read(new_socket, buffer, sizeof(buffer));

        pid_t pid = fork();

        if(pid == 0){
            // CHILD PROCESS

            if(option == 1){

                sprintf(result,
                "Handled by CHILD 1 (PID %d)\n"
                "Name: John Doe\n"
                "Address: 21, Park Street, Bangalore",
                getpid());
            }

            else if(option == 2){

                sprintf(result,
                "Handled by CHILD 2 (PID %d)\n"
                "Dept: Computer Science\n"
                "Semester: 5\n"
                "Section: A\n"
                "Courses: OS, CN, DBMS",
                getpid());
            }

            else if(option == 3){

                sprintf(result,
                "Handled by CHILD 3 (PID %d)\n"
                "Marks Obtained: 87",
                getpid());
            }

            // Send result to client
            write(new_socket, result, sizeof(result));

            exit(0); // terminate child
        }
        else{
            // Parent waits to avoid zombie processes
            wait(NULL);
        }
    }

    close(new_socket);
    close(server_fd);
}
