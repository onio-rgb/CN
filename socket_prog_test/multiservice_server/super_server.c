#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    bind(server_fd,(struct sockaddr *) &addr, sizeof(addr));

    listen(server_fd, 20);

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = 0;
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        printf("Servicing the client with address %s port %d %d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),client_addr_len);
        int c = fork();
        if (c > 0)
        {
            close(client_fd);
            struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
        }
        else
        {
            close(server_fd);
            dup2(client_fd,0);
            dup2(client_fd,1);
            dup2(client_fd,2);
            char *argv[1] = {NULL};
            execvp("./service",argv);
        }
    }
}