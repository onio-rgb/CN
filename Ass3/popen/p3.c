#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
    int fd[2];
    pipe(fd);
    int c = fork();
    if (c > 0)
    {
        close(fd[0]);
        char buff[100];
        scanf("%s", buff);
        fflush(stdin);
        printf("P3 : Read %s from P2\n", buff);
        printf("P3 : Writing %s to P4\n", buff);
        fflush(stdout);
        dup2(fd[1], 1);
        printf("%s", buff);
        fflush(stdout);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], 0);
        char *argv[] = {NULL};
        execvp("./p4", argv);
        printf("wrong execvp p3\n");
    }
}