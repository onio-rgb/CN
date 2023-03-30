#include<stdlib.h>
int main()
{
    system("gcc client.c -o client");
    system("gcc super_server.c -o ss");
    system("gcc s1.c -o s1");
    system("gcc s2.c -o s2");
    system("gcc s3.c -o s3");
    system("gcc s4.c -o s4");
}