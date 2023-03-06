#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
int main()
{
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_len;
    char *buff = malloc(200);
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    while (1)
    {
        recvfrom(sfd, buff, 200, 0, (struct sockaddr *)&client_addr, &client_addr_len);

        printf("recieved %s from client\n", buff);
    }
}