#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
int main()
{
    srand(time(NULL));
    sleep(1+rand()%10);
    printf("Hello I am from p2\n");
    fflush(stdout);
}