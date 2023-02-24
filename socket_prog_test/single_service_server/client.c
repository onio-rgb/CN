#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char * args[])
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    socklen_t server_addr_len = sizeof(server_addr);
    connect(client_fd, (struct sockaddr *)&server_addr, server_addr_len);

    char path[1024];
    sprintf(path,"/home/onio/Documents/CN/socket_prog_test/multiservice_server/%s",args[1]);
    send(client_fd, path, sizeof(path), 0);

    char *conform = malloc(200);
    recv(client_fd, conform, 200, 0);
    printf("%s", conform);

    char *content = malloc(2048);
    recv(client_fd, content, 2048, 0);
    printf("%s\n",content);
}