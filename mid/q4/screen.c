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
#include <sys/select.h>
void reuseaddr(int fd)
{
    int *val = malloc(sizeof(int));
    *val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, val, sizeof(int));
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, val, sizeof(int));
}
int main()
{
    int news_reader_fds[2];
    fd_set readfds;
    FD_ZERO(&readfds);
    for (int i = 0; i < 2; i++)
    {
        news_reader_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in news_reader_addr;
        news_reader_addr.sin_addr.s_addr = INADDR_ANY;
        news_reader_addr.sin_family = AF_INET;
        news_reader_addr.sin_port = htons(9000 + i);
        reuseaddr(news_reader_fds[i]);
        bind(news_reader_fds[i], (struct sockaddr *)&news_reader_addr, sizeof(news_reader_addr));
        listen(news_reader_fds[i], 2);
    }
    int nsfd[2];
    for (int i = 0; i < 2; i++)
    {
        struct sockaddr client_addr;
        socklen_t client_len;
        nsfd[i] = accept(news_reader_fds[i], &client_addr, &client_len);
        FD_SET(nsfd[i], &readfds);
    }

    while (1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (FD_ISSET(nsfd[i], &readfds))
            {
                char msg[300];
                recv(nsfd[i], msg, 300, 0);
                printf("%s\n", msg);
            }
        }
        FD_ZERO(&readfds);
        for (int i = 0; i < 2; i++)
            FD_SET(nsfd[i], &readfds);
    }
}