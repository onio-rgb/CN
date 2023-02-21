#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<errno.h>
char * pipe12="/home/onio/12";
char * pipe23="/home/onio/23";

int main()
{
    int errno;
    printf("this is process P2\n");
    unlink(pipe23);
    mkfifo(pipe23,0666);
    // printf("%d\n",p23);
    int f=fork();
    if(f>0)
    {
        int p12=open(pipe12,O_RDONLY);
        int p23=open(pipe23,O_WRONLY);
        char b[20];
        read(p12,b,20);
        close(p12);
        printf("P2 : P1 sent %s\n",b);
        write(p23,b,sizeof(b));
        close(p23);

        // sleep(2);
        p23=open(pipe23,O_RDONLY);
        char c[20];
        read(p23,c,sizeof(c));
        printf("P2 : P3 sent %s\n",c);
        p12=open(pipe12,O_WRONLY);
        write(p12,c,sizeof(c));
        close(p12);
    }
    else 
    {
        // sleep(2);
        char * args[1]={NULL};
        execvp("./p3",args);
    }

}