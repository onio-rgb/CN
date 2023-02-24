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
int main(int argc, char *args[])
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);
    socklen_t server_addr_len = sizeof(server_addr);
    connect(client_fd, (struct sockaddr *)&server_addr, server_addr_len);

    struct nums num;
    num.a = 10;
    num.b = 20;
    send(client_fd, &num, sizeof(num), 0);

    int res = 0;
    recv(client_fd, &res, sizeof(res), 0);
    printf("Client 1 %d\n", res);
}