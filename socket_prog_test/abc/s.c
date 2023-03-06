#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int sock_fd[4];
    for(int i=0;i<4;i++)
    {
        sock_fd[i] = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addr;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(8080);
        addr.sin_family = AF_INET;

        bind(sock_fd[i], (struct sockaddr *)&addr, sizeof(addr));

        listen(sock_fd[i], 20);
    }
    
}