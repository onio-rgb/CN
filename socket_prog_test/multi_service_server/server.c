#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <errno.h>
int errno;
int main()
{
    fd_set read_sock;
    FD_ZERO(&read_sock);
    int sock_fd[4];
    for (int i = 0; i < 4; i++)
    {
        sock_fd[i] = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8080 + i);
        bind(sock_fd[i], (struct sockaddr *)&server_addr, sizeof(server_addr));
        listen(sock_fd[i], 20);
        FD_SET(sock_fd[i], &read_sock);
    }
    while (1)
    {
        perror("");
        int ready = select(100, &read_sock, NULL, NULL, NULL);
        printf("%d\n", ready);
        for (int i = 0; i < 4; i++)
        {
            if (FD_ISSET(sock_fd[i], &read_sock))
            {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = 0;
                int client_fd = accept(sock_fd[i], (struct sockaddr *)&client_addr, &client_addr_len);
                int c = fork();
                if (c > 0)
                {
                    close(client_fd);
                }
                else
                {
                    for (int j = 0; j < 4; j++)
                        close(sock_fd[j]);
                    dup2(client_fd, 0);
                    dup2(client_fd, 1);
                    dup2(client_fd, 2);
                    char *argv[1] = {NULL};
                    char *service = malloc(20);
                    sprintf(service, "./service%d", i);
                    execvp(service, argv);
                }
            }
        }
        for (int i = 0; i < 4; i++)
        {
            FD_SET(sock_fd[i], &read_sock);
        }
    }
}