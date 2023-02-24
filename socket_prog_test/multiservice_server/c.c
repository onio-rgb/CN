#include<stdlib.h>
int main()
{
    system("gcc client.c -o client");
    system("gcc service.c -o service");
    system("gcc super_server.c -o ss");
}