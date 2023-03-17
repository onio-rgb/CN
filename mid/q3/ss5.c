#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <sys/msg.h>
int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8084);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 20);

    struct sockaddr proxy_addr;
    socklen_t len = sizeof(proxy_addr);
    int proxy_fd = accept(server_fd, &proxy_addr, &len);

    while (1)
    {

        char *buff = malloc(100);
        recv(proxy_fd, buff, 100, 0);
        buff = malloc(100);
        sprintf(buff, "Serviced by server 5\n");
        send(proxy_fd, buff, 100, 0);
    }
}