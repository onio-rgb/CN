#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    system("gcc p1.c -o p1");
    system("gcc p2.c -o p2");
    system("gcc p3.c -o p3");
    system("gcc p4.c -o p4");
}