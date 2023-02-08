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
        sleep(1);
        close(fd[0]);
        printf("Writing 1 from p1 : \n");
        fflush(stdout);
        dup2(fd[1], 1);
        printf("1");
        fflush(stdout);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], 0);
        // char *pipe_fd_read[10];
        // snprintf(pipe_fd_read, 10, "%d", fd[0]);
        char *argv[] = {NULL};
        execvp("./p2", argv);
        printf("wrong execvp p1\n");
    }
}