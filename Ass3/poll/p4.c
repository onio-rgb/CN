#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
int main()
{
    srand(time(NULL));
    sleep(1+rand()%4);
    printf("Hello I am from p4\n");
    fflush(stdout);
}