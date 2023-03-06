#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
int main()
{
    struct sockaddr_in serv_addr;
    char *buff = malloc(200);
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    buff="Hellp thsi is cleint";
    sendto(cfd, buff, 200, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

}