#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
    system("gcc p1.c -o p1");
    system("gcc p2.c -o p2");
    system("gcc p3.c -o p3");
    system("gcc p4.c -o p4");
    system("./p2");
}