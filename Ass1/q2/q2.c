#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int fdp2c[2];
    int fdc2p[2];
    pipe(fdp2c);
    pipe(fdc2p);
    int stdin_cpy=dup(0);
    int stdout_cpy=dup(1);
    int c = fork();
    if (c > 0)
    {
        close(fdp2c[0]);
        close(fdc2p[1]);
        char *buff = malloc(20);
        while (1)
        {
            printf("Enter for parent :");
            fgets(buff, 20, stdin);
            if (buff[0] == '\n')
                break;
            write(fdp2c[1], buff, 20);
            read(fdc2p[0], buff, 20);
            printf("Parent read : %s", buff);
        }
    }
    else
    {
        close(fdp2c[1]);
        close(fdc2p[0]);
        char s1[10];
        char s2[10];
        sprintf(s1,"%d",stdin_cpy);
        sprintf(s2,"%d",stdout_cpy);
        dup2(fdp2c[0],0);
        dup2(fdc2p[1],1);
        char *args[3]={s1,s2,NULL};
        execvp("./p",args);
        write(stdout_cpy,"asa",3);
    }
}