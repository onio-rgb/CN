#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char * pipe23="/home/onio/23";
int main()
{
    printf("this is process P3\n");
    int p23=open(pipe23,O_RDONLY);
    char b[20];
    read(p23,b,20);
    close(p23);
    printf("P3 : P2 sent %s\n",b);
    printf("P3 : P3 displating p2 p1 msg %s\n",b);
    printf("Enter for P3 : \n");
    char c[20];
    scanf("%s",c);
    p23=open(pipe23,O_WRONLY);
    write(p23,c,sizeof(c));
    close(p23);
}