#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int stdin_cpy = atoi(argv[0]);
    int stdout_cpy = atoi(argv[1]);
    char *buff = malloc(20);
    while (1)
    {
        fgets(buff, 20, stdin); // pipe I
        fflush(stdin);
        int pipWritefd = dup(1);
        dup2(stdout_cpy, 1);            // change to screen O
        printf("P2 Read : %s\n", buff); // screen O
        printf("Enter for P2 : ");
        fflush(stdout);
        dup2(stdin_cpy, 0);     // change to keyboard I
        fgets(buff, 20, stdin); // keyboard I
        fflush(stdin);
        if (buff[0] == '\n')
            break;
        dup2(pipWritefd, 1); // change to pipe O
        printf("%s", buff);  // write to pipe O
        fflush(stdout);
    }
}