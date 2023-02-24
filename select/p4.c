#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
int main()
{
    srand(time(NULL));
    sleep(2+rand()%1);
    printf("Hello I am from p4\n");
    fflush(stdout);
}