#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main()
{
    int fdp2c[2];
    int fdc2p[2];
    pipe(fdp2c);
    pipe(fdc2p);
    int c = fork();
    if (c > 0)
    {
        close(fdp2c[0]);
        close(fdc2p[1]);
        char *buff = malloc(20);
        while (1)
        {
            printf("Enter for Parent\n");
            fgets(buff,sizeof(buff),stdin);
            if (buff[0]=='\n')
                break;
            write(fdp2c[1], buff, 20);
            
            read(fdc2p[0], buff, 20);
            printf("Parent Read : %s\n", buff);
        }
    }
    else
    {
        close(fdp2c[1]);
        close(fdc2p[0]);
        char *buff = malloc(20);
        while (1)
        {
            read(fdp2c[0], buff, 20);
            printf("Child Read : %s\n", buff);
            printf("Enter for child\n");
            fgets(buff,sizeof(buff),stdin);
            if (buff[0]=='\n')
                break;
            write(fdc2p[1], buff, 20);
            
        }
    }
}