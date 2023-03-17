#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <poll.h>
int main()
{
    struct pollfd *fds = malloc(sizeof(struct pollfd) * 4);
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
        fds[i].fd = sock_fd[i];
        fds[i].events = POLL_IN;
    }
    while (1)
    {
        int ready = poll(fds, 4, -1);
        // perror("");
        // printf("%d\n",ready);
        for (int i = 0; i < 4; i++)
        {
            if (fds[i].revents & POLL_IN)
            {
                int c = fork();
                if (c == 0)
                {
                    char *sock_fd_string = malloc(20);
                    sprintf(sock_fd_string, "%d", sock_fd[i]);
                    char *arg[2] = {sock_fd_string, NULL};
                    char path[10];
                    sprintf(path, "./s%d", i + 1);
                    execvp(path, arg);
                    printf("Execv failed\n");
                }
                fds[i].fd = -1;
            }
        }
    }
}