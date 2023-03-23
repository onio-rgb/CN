#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/select.h>
#define ADDRESS "mysocket"
int main()
{
    int usfd=socket(AF_UNIX,SOCK_STREAM,0);
    
}