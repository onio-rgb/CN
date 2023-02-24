#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
int main()
{
    srand(time(NULL));
    sleep(5+rand()%4);
    printf("Hello I am from p3\n");
    fflush(stdout);
}