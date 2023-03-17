#include <stdlib.h>
int main()
{
    system("gcc client1.c -o client1");
    system("gcc client2.c -o client2");
    system("gcc proxy.c -o proxy");
    system("gcc ss1.c -o ss1");
    system("gcc ss2.c -o ss2");
    system("gcc ss3.c -o ss3");
    system("gcc ss4.c -o ss4");
    system("gcc ss5.c -o ss5");
}