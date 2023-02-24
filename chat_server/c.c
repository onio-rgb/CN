#include <stdlib.h>
int main()
{
    system("gcc chat_server.c -o s");
    system("gcc client0.c -o client0");
    system("gcc client1.c -o client1");
    system("gcc client2.c -o client2");
    system("gcc client3.c -o client3");
}