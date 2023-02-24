#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
int main()
{
    srand(time(NULL));
    sleep(3+rand()%2);
    printf("Hello I am from p5\n");
    fflush(stdout);
}