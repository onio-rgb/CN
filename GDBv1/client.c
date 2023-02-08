#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include<string.h>
#define FIFO "/tmp/read_cpp"
int main()
{
    char *s=malloc(1024);
    int fifofd=open(FIFO,O_RDWR);
    while(scanf("%[^$$]s",s)==1){
        write(fifofd,s,strlen(s));
    }
    sleep(1);
    char *verdict=malloc(30);
    int n_read=read(fifofd,verdict,30);
    if(n_read>0)
    {
        printf("%s\n",verdict);
    }
    else {
        printf("Error in receiving verdict\n");
    }
}