#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
struct nums
{
    int a;
    int b;
};
int main()
{
    struct nums in;
    recv(0, &in, sizeof(in), 0);
    int res = in.a * in.b;
    send(1,&res,sizeof(res),0);
}