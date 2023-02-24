#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
int main()
{
    int server_fd=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8080);

    bind(server_fd,&server_addr,sizeof(server_addr));
    listen(server_fd,2);
    
}