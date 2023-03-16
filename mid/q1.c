#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/ipc.h>
#include<arpa/inet.h>
#include<signal.h>
int main()
{
    int service_sock_fds[4];
    for(int i=0;i<4;i++)
    {
        service_sock_fds[i]=socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in server_addr;
        server_addr.sin_addr.s_addr=INADDR_ANY;
        server_addr.sin_family=AF_INET;
        server_addr.sin_port=htons(8080);
        bind(service_sock_fds[i],&server_addr,sizeof(server_addr));
        listen(service_sock_fds[i],2);
    }
    
}