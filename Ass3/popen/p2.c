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
        // int n_read=read(0,buff,100);
        scanf("%s",buff);
        fflush(stdin);
        // printf("%d\n",n_read);
        // if(n_read==-1){
        //     printf("Error reading pipe p2\n");
        //     return 1;
        // }
        printf("P2 : Read %s from P1\n",buff);
        printf("P2 : Writing %s to P3\n",buff);
        fflush(stdout);
        dup2(fd[1],1);
        printf("%s",buff);
        fflush(stdout);
    }
    else
    {
        
        close(fd[1]);
        
        dup2(fd[0], 0);
        char *argv[] = {NULL};
        execvp("./p3", argv);
        printf("wrong execvp p2\n");
    }
}