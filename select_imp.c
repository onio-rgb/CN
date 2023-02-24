#include<stdio.h>
#include<stdlib.h>
#include<sys/select.h>
#include<unistd.h>
int main()
{
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd1,&read_set);
}