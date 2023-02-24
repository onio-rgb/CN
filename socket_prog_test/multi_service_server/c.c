#include<stdlib.h>
int main()
{
    system("gcc client0.c -o client0");
    system("gcc service0.c -o service0");
    system("gcc client1.c -o client1");
    system("gcc service1.c -o service1");
    system("gcc client2.c -o client2");
    system("gcc service2.c -o service2");
    system("gcc client3.c -o client3");
    system("gcc service3.c -o service3");
    system("gcc server.c -o ss");
}