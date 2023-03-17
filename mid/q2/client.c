#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int off = atoi(argv[1]);
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080 + off);
    connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    perror("");
    char buff[20];
    recv(sock_fd, buff, 20, 0);
    printf("client %d receive service %d\n", off, off+1);
}