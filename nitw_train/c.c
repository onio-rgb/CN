#include<stdlib.h>
int main()
{
    system("gcc p1.c -o p1");
    system("gcc p2.c -o p2");
    system("gcc p3.c -o p3");
    system("gcc sm.c -o sm");
    system("gcc t1.c -o t1");
    system("rm plat1");
    system("rm plat2");
    system("rm plat3");
}