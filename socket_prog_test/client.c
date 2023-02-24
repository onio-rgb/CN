#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
int errno;
int main()
{
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_port = htons(8080);
    server_addr.sin_family = AF_INET;
    struct in_addr ip;
    inet_pton(AF_INET, "127.0.0.1", &ip);
    server_addr.sin_addr = ip;
    connect(cfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    send(cfd, "This is the cliend hello\n", sizeof("This is the cliend hello\n"), 0);
    char  conf[200];
    recv(cfd,conf,200,0);
    printf("%s",conf);
}