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
#include <errno.h>
int createTCP(int port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    int k=1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &k, sizeof(int));
    setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &k, sizeof(int));
    bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sfd, 20);
    return sfd;
}
int connectTCP(int port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    return sfd;
}