#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<errno.h>
char * pipe12="/home/onio/12";
int main()
{
    int errno;
    unlink(pipe12);
    int x=mkfifo(pipe12,0666);
    int c=fork();
    if(c>0)
    {
        int p12=open(pipe12,O_WRONLY );
        char b[20];
        printf("Enter p1 : \n");
        scanf("%s",b);
        write(p12,b,20);
        close(p12);
        // sleep(4);
        p12=open(pipe12,O_RDONLY);
        read(p12,b,20);
        close(p12);
        printf("P1 : P2 sent %s\n",b);
    }
    else 
    {
        // sleep(1);
        char * args[1]={NULL};
        execvp("./p2",args);
    }
}