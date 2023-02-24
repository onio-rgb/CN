#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<errno.h>
int errno;
int main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr ;
    server_addr.sin_port=htons(8080);
    server_addr.sin_family=AF_INET;
    struct in_addr n_ip;
    inet_pton(AF_INET,"127.0.0.1",&n_ip);
    server_addr.sin_addr=n_ip;
    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("bind fail\n");
        perror("");
        return 1;
    }
    if (listen(sfd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1){    
        struct sockaddr *client_addr = malloc(sizeof(struct sockaddr));
        socklen_t client_len = 0;
        int cfd = accept(sfd, client_addr, &client_len);
        perror("");
        char *buff = malloc(200);
        int n = recv(cfd,buff,200,0);
        printf("Client sent %s\n",buff);
        send(cfd,"Message received\n",sizeof("Message received\n"),0);
    }
}