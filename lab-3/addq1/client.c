/*
    PROGRAM: Menu Driven Calculator Client using TCP

    DESCRIPTION:
    Client displays menu, takes input from user,
    sends data to server, and displays results.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX 10

int main(){

    int sock, choice;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    while(1){

        printf("\nMENU");
        printf("\n1.Add/Subtract");
        printf("\n2.Linear Equation");
        printf("\n3.Matrix Multiply");
        printf("\n4.Exit");

        printf("\nEnter choice: ");
        scanf("%d",&choice);

        write(sock,&choice,sizeof(choice));

        if(choice==4)
            break;

        // ============ ADD / SUBTRACT ============
        if(choice==1){

            int a,b,result[2];

            printf("Enter two integers: ");
            scanf("%d%d",&a,&b);

            write(sock,&a,sizeof(a));
            write(sock,&b,sizeof(b));

            read(sock,result,sizeof(result));

            printf("Addition: %d\n",result[0]);
            printf("Subtraction: %d\n",result[1]);
        }

        // ============ LINEAR ============
        else if(choice==2){

            float a,b,x;

            printf("Equation format: ax + b = 0\n");
            printf("Enter a and b: ");
            scanf("%f%f",&a,&b);

            write(sock,&a,sizeof(a));
            write(sock,&b,sizeof(b));

            read(sock,&x,sizeof(x));

            printf("Value of x = %.2f\n",x);
        }

        // ============ MATRIX ============
        else if(choice==3){

            int r1,c1,r2,c2;
            int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

            printf("Enter rows and cols of Matrix A: ");
            scanf("%d%d",&r1,&c1);

            printf("Enter rows and cols of Matrix B: ");
            scanf("%d%d",&r2,&c2);

            if(c1 != r2){
                printf("Matrix multiplication not possible!\n");
                continue;
            }

            printf("Enter Matrix A:\n");
            for(int i=0;i<r1;i++)
                for(int j=0;j<c1;j++)
                    scanf("%d",&A[i][j]);

            printf("Enter Matrix B:\n");
            for(int i=0;i<r2;i++)
                for(int j=0;j<c2;j++)
                    scanf("%d",&B[i][j]);

            write(sock,&r1,sizeof(r1));
            write(sock,&c1,sizeof(c1));
            write(sock,&r2,sizeof(r2));
            write(sock,&c2,sizeof(c2));

            write(sock,A,sizeof(A));
            write(sock,B,sizeof(B));

            read(sock,C,sizeof(C));
            read(sock,&r1,sizeof(r1));
            read(sock,&c2,sizeof(c2));

            printf("Result Matrix:\n");

            for(int i=0;i<r1;i++){
                for(int j=0;j<c2;j++){
                    printf("%d ",C[i][j]);
                }
                printf("\n");
            }
        }
    }

    close(sock);
}
