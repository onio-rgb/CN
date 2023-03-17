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
int main(int c,char * argv[])
{
    int i=atoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in proxy_addr;
    proxy_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    proxy_addr.sin_port = htons(9090);
    proxy_addr.sin_family = AF_INET;

    connect(server_fd, (struct sockaddr *)&proxy_addr, sizeof(proxy_addr));
    int *server_no = malloc(sizeof(int));
    *server_no =  i+1;
    send(server_fd, server_no, sizeof(int), 0);
    char *buff = malloc(100);
    sprintf(buff, "Client 1 : Service client %d \n", *server_no);
    send(server_fd, buff, 100, 0);
    buff = malloc(100);
    recv(server_fd, buff, 100, 0);
    printf("client 1 : %s", buff);
    shutdown(server_fd,SHUT_RDWR);
    close(server_fd);
}